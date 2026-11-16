# Copyright 2026 DigitalExpressions Sweden
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Independent rational/committed-Decimal SIMD corpus and comparison policies."""
import argparse
from fractions import Fraction
import hashlib
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
DIRECTORY = ROOT / 'tests/cross_platform'
SCHEMA = 1
SEED = 1592594996


def encoded(value):
    return (json.dumps(value, sort_keys=True, separators=(',', ':')) + '\n').encode('utf-8')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def word(value, width):
    return format(value & ((1 << width) - 1), '0' + str(width // 4) + 'x')


def scalar(bits, width):
    return struct.unpack('>f' if width == 32 else '>d', int(bits, 16).to_bytes(width // 8, 'big'))[0]


def rational(bits, width):
    """Decode finite IEEE bits exactly, including subnormals, without libm."""
    raw = int(bits, 16)
    fraction_bits, bias = (23, 127) if width == 32 else (52, 1023)
    exponent = (raw >> fraction_bits) & (2 * bias + 1)
    mantissa = raw & ((1 << fraction_bits) - 1)
    if exponent == 2 * bias + 1:
        raise ValueError('Non-finite input to rational oracle')
    power = (exponent or 1) - bias - fraction_bits
    significand = mantissa + ((1 << fraction_bits) if exponent else 0)
    result = Fraction(significand) * Fraction(2) ** power
    return -result if raw >> (width - 1) else result


def rounded(value, width, negative_zero=False):
    """Round an exact rational to IEEE nearest-even without double rounding."""
    fraction_bits, bias = (23, 127) if width == 32 else (52, 1023)
    sign = int(value < 0 or (value == 0 and negative_zero)) << (width - 1)
    value = abs(value)
    if not value:
        return word(sign, width)
    exponent = value.numerator.bit_length() - value.denominator.bit_length()
    if value < Fraction(2) ** exponent:
        exponent -= 1
    exponent = max(exponent, 1 - bias)
    scaled = value / Fraction(2) ** (exponent - fraction_bits)
    mantissa, remainder = divmod(scaled.numerator, scaled.denominator)
    if 2 * remainder > scaled.denominator or (2 * remainder == scaled.denominator and mantissa & 1):
        mantissa += 1
    if mantissa == 1 << (fraction_bits + 1):
        exponent += 1
        mantissa >>= 1
    if exponent > bias:
        return word(sign | ((2 * bias + 1) << fraction_bits), width)
    field = exponent + bias if mantissa >= 1 << fraction_bits else 0
    return word(sign | (field << fraction_bits) | (mantissa & ((1 << fraction_bits) - 1)), width)


def generate():
    cases = []

    def add(kind, width, op, a, b=None, c=None, expected=None, policy='exact', mode=0, parameter=0, variant=0):
        bits = int(kind[1:])
        lanes = width // bits
        cycle = lambda values: [values[i % len(values)] for i in range(lanes)]
        a = cycle(a)
        b = cycle(b or [word(0, bits)])
        c = cycle(c or [word(0, bits)])
        record = dict(id=f'{kind}-w{width}-{op}-r{mode}-p{parameter}-v{variant}',
                      type=kind, width=width, op=op, rounding=mode, parameter=parameter,
                      policy=policy, a=a, b=b, c=c)
        record['expected'] = expected(a, b, c) if callable(expected) else expected
        cases.append(record)

    state = SEED
    random_words = []
    for _ in range(16):
        state ^= (state << 13) & 0xffffffff
        state ^= state >> 17
        state ^= (state << 5) & 0xffffffff
        state &= 0xffffffff
        random_words.append(state)
    for width in (128, 256, 512):
        for kind in ('i32', 'u32', 'i64', 'u64'):
            bits = int(kind[1:])
            mask = (1 << bits) - 1
            values = [0, 1, mask, 1 << (bits - 1), (1 << (bits - 1)) - 1, mask - 1, *random_words]
            aa = [word(x, bits) for x in values]
            bb = [word(x, bits) for x in values[3:] + values[:3]]
            signed = lambda n: n - (1 << bits) if kind[0] == 'i' and n >> (bits - 1) else n
            for op in ('load', 'add', 'sub', 'mul', 'xor', 'shift_left', 'shift_right', 'lt', 'select'):
                def expected(a, b, c, op=op, bits=bits, mask=mask, signed=signed):
                    result = []
                    for x, y in zip(map(lambda s: int(s, 16), a), map(lambda s: int(s, 16), b)):
                        value = {'load': lambda: x, 'add': lambda: x + y, 'sub': lambda: x - y,
                                 'mul': lambda: x * y, 'xor': lambda: x ^ y,
                                 'shift_left': lambda: x << 3, 'shift_right': lambda: signed(x) >> 3,
                                 'lt': lambda: int(signed(x) < signed(y)),
                                 'select': lambda: x if signed(x) < signed(y) else y}[op]()
                        result.append(word(value & mask, bits))
                    return result
                for variant in range(2):
                    add(kind, width, op, aa[variant * 4:], bb[variant * 4:], expected=expected, variant=variant)
        for kind in ('f32', 'f64'):
            bits = int(kind[1:])
            sign = 1 << (bits - 1)
            frac, bias = (23, 127) if bits == 32 else (52, 1023)
            infinity = (2 * bias + 1) << frac
            special = [0, sign, 1, sign | 1, (1 << frac) - 1, 1 << frac,
                       infinity - 1, infinity, infinity | sign, infinity | (1 << (frac - 1)) | 0x123,
                       infinity | 1, sign | infinity | (1 << (frac - 1)) | 0x456]
            aa = [word(x, bits) for x in special]
            # Raw bit movement covers every special input even at the smallest width.
            for variant in range(0, len(aa), width // bits):
                block = (aa + aa)[variant:variant + width // bits]
                add(kind, width, 'load', block, expected=lambda a, b, c: a, variant=variant)
                add(kind, width, 'neg', block, expected=lambda a, b, c: [word(int(x, 16) ^ sign, bits) for x in a], variant=variant)
                add(kind, width, 'classify', block,
                    expected=lambda a, b, c: [word(1 if (int(x, 16) & ((1 << frac) - 1)) and (int(x, 16) & infinity) == infinity
                                                   else 2 if (int(x, 16) & ~sign) == infinity
                                                   else 4 if (int(x, 16) & ~sign) == 0
                                                   else 8 if (int(x, 16) & infinity) == 0 else 16, bits) for x in a], variant=variant)
            finite = [rounded(Fraction(n, 8), bits) for n in (0, 1, -1, 20, -20, 56, -56, 11, 23, -13)]
            other = [rounded(Fraction(n, 4), bits) for n in (1, 3, -3, 5, -5, 7, 13, -17)]
            for op in ('add', 'sub', 'mul', 'div', 'min', 'max', 'lt', 'select'):
                def fp_expected(a, b, c, op=op, bits=bits):
                    result = []
                    for x, y in zip(a, b):
                        p, q = rational(x, bits), rational(y, bits)
                        if op == 'lt':
                            value = word(int(p < q), bits)
                        elif op in ('min', 'max', 'select'):
                            value = x if (p < q if op != 'max' else p > q) else y
                        else:
                            value = rounded({'add': lambda: p + q, 'sub': lambda: p - q,
                                             'mul': lambda: p * q, 'div': lambda: p / q}[op](), bits,
                                            negative_zero=op in ('mul', 'div') and bool((int(x, 16) ^ int(y, 16)) >> (bits - 1)))
                        result.append(value)
                    return result
                add(kind, width, op, finite, other, expected=fp_expected)
                boundary=[word(x,bits) for x in (1,sign|1,1<<frac,(1<<frac)-1,infinity-1,sign|(1<<frac))]
                add(kind, width, op, boundary, other, expected=fp_expected, variant=1)
                seeded=[word(((bias-2+i%5)<<frac) | (n & ((1<<frac)-1)) | (sign if i%2 else 0),bits)
                        for i,n in enumerate(random_words)]
                add(kind, width, op, seeded, other, expected=fp_expected, variant=2)
            # Every defined partial count; inactive store lanes retain raw canaries.
            for count in range(width // bits + 1):
                add(kind, width, 'load_partial', aa, expected=lambda a, b, c, n=count: a[:n] + [word(0, bits)] * (len(a) - n), parameter=count)
                add(kind, width, 'store_partial', aa, expected=lambda a, b, c, n=count: a[:n] + [word(0x1234, bits)] * (len(a) - n), parameter=count)
            ties = [rounded(Fraction(n, 2), bits) for n in (-5, -3, -1, 1, 3, 5, 7, -7)]
            for mode in range(4):
                def converted(a, b, c, mode=mode, bits=bits):
                    values = [rational(x, bits) for x in a]
                    convert = (round, lambda q: q.numerator // q.denominator,
                               lambda q: -((-q.numerator) // q.denominator), int)[mode]
                    return dict(current=[word(convert(q), bits) for q in values],nearest=[word(round(q),bits) for q in values])
                add(kind, width, 'roundi', ties, expected=converted, mode=mode, policy='roundi')
            epsilon = Fraction(1, 1 << frac)
            a = [rounded(1 + epsilon, bits), rounded(-1 - epsilon, bits)]
            b = [rounded(1 - epsilon, bits)]
            c = [rounded(-1, bits), rounded(1, bits)]
            def fma_expected(a, b, c, bits=bits):
                fused, split = [], []
                for x, y, z in zip(a, b, c):
                    p, q, r = rational(x, bits), rational(y, bits), rational(z, bits)
                    fused.append(rounded(p * q + r, bits))
                    split.append(rounded(rational(rounded(p * q, bits), bits) + r, bits))
                return dict(fused=fused, unfused=split)
            add(kind, width, 'mul_add', a, b, c, expected=fma_expected, policy='fma')
            if bits == 32:
                add(kind, width, 'approx_recipr', [x for x in other if rational(x, 32) > 0], expected=lambda a, b, c: [rounded(1 / rational(x, 32), 32) for x in a], policy='reciprocal')
            # Existing independently generated Decimal fixtures, restricted to a shared bounded domain.
            for family, operations in [('exp_log', ['exp','expm1','exp2','exp10','log','log1p','log2','log10']),
                                       ('trig', ['sin','cos','tan'])]:
                text = (ROOT / 'tests' / (family + '_reference.h')).read_text(encoding='utf-8')
                rows = re.findall(r'\{(32|64),(\d+),0x([0-9a-f]+)ull,0x([0-9a-f]+)ull\}', text)
                for index, op in enumerate(operations):
                    selected = [(word(int(x,16),bits),word(int(y,16),bits)) for w,o,x,y in rows
                                if int(w)==bits and int(o)==index and Fraction(1,16) <= rational(word(int(x,16),bits),bits) <= 1]
                    if not selected:
                        raise ValueError('Missing Decimal fixture selection: ' + op)
                    chosen = [selected[i * (len(selected)-1) // 15] for i in range(16)]
                    for variant in range(2):
                        group = chosen[variant * 8:]
                        expected = [group[i % len(group)][1] for i in range(width // bits)]
                        add(kind, width, op, [x for x,y in group], expected=expected,
                            policy='ulp' if family=='exp_log' else 'trig', variant=variant)
    ids = [case['id'] for case in cases]
    if len(ids) != len(set(ids)):
        raise ValueError('Duplicate corpus IDs')
    return cases


def load():
    policies = json.loads((DIRECTORY / 'policies.json').read_text(encoding='utf-8'))
    cases = [json.loads(line) for line in (DIRECTORY / 'corpus.jsonl').read_text(encoding='utf-8').splitlines()]
    if policies['schema'] != SCHEMA or policies['version']!='simd-policies-2' or policies['seed'] != SEED or set(policies['policies']) != {'exact','fma','roundi','ulp','trig','reciprocal'}:
        raise ValueError('Unknown corpus schema, seed or policies')
    if cases != generate():
        raise ValueError('Corpus differs from independent generator; review and regenerate deliberately')
    return cases, policies


def input_bytes(cases, max_width):
    return ''.join(' '.join([c['id'],c['type'],str(c['width']),c['op'],str(c['rounding']),str(c['parameter']),c['policy'],
                            *c['a'],*c['b'],*c['c']])+'\n' for c in cases if c['width']<=max_width).encode('ascii')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    content = b''.join(encoded(case) for case in generate())
    path = DIRECTORY / 'corpus.jsonl'
    if args.write:
        path.write_bytes(content)
    load()
    if path.read_bytes() != content:
        raise ValueError('Corpus must use canonical UTF-8/LF serialization')
    print(json.dumps(dict(cases=len(generate()), corpus_sha256=digest(path), policies_sha256=digest(DIRECTORY/'policies.json'))))


if __name__ == '__main__':
    main()
