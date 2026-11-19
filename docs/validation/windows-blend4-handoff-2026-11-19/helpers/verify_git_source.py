"""Verify a clean pinned Git publication against its blobs and the original M2 capture.
A later documentation publication never acquires execution at the old capture ID.
"""
from pathlib import Path
import argparse,hashlib,json,subprocess
ANCHOR='3ae63872ad69d4fb7310f7ff07abbe40068a2ff29c93a34f666be0b4602c0e36'
POLICY='docs/validation/windows-blend4-handoff-2026-11-19/git-source-policy.json'
def digest(data):return hashlib.sha256(data).hexdigest()
def compare_original(root,names):
 policy=json.loads((root/POLICY).read_text());original=json.loads((root/policy['original_manifest']).read_text());rows=original['files']
 fingerprint=digest(''.join(r['path']+'\0'+r['sha256']+'\n' for r in rows).encode())
 if len(rows)!=571 or fingerprint!=ANCHOR or original['fingerprint']!=ANCHOR:raise ValueError('Original M2 manifest anchor mismatch')
 overrides={r['path']:r for r in policy['metadata_changes']};actual=[]
 allowed={'.gitattributes','AGENTS.md','README-vcl2-xa64.md','docs/integration-plan.md','docs/validation/README.md'}
 if set(overrides)-allowed:raise ValueError('An implementation path cannot be a metadata allowance')
 if any(not n.startswith('docs/') for n in policy['added_paths']):raise ValueError('Unexpected addition outside report/validation metadata')
 for row in rows:
  name=row['path'];data=(root/name).read_bytes();value=digest(data)
  if value!=row['sha256']:
   expected=overrides.get(name)
   if not expected or expected['original_sha256']!=row['sha256'] or expected['publication_sha256']!=value:raise ValueError('Unlisted original-source change: '+name)
   actual.append(name)
 if sorted(actual)!=sorted(overrides):raise ValueError('Metadata change inventory mismatch')
 original_names={r['path'] for r in rows};added=sorted(set(names)-original_names)
 if set(names)&original_names!=original_names or added!=policy['added_paths'] or len(names)!=policy['publication_files']:raise ValueError('Complete publication inventory mismatch')
 pin=json.loads((root/'vendor/vectorclass.json').read_text())
 if pin['commit']!='667577495efaa36a10cbb5d6cb8c942c23913848' or pin['tree']!='36aee120c28c9c64e62903bfc81139003e37cedd':raise ValueError('Protected upstream pin changed')
 for row in pin['files']:
  for name in (row['path'],'vendor/vectorclass/'+row['path']):
   if digest((root/name).read_bytes())!=row['sha256']:raise ValueError('Protected raw-byte mismatch: '+name)
 return dict(original_capture_fingerprint=ANCHOR,original_files=571,publication_files=len(names),metadata_changes=actual,added_paths=added,all_runtime_tests_tools_fixtures_and_build_inputs_match=True)
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--checkout',type=Path,required=True);p.add_argument('--expected-commit',required=True);p.add_argument('--output',type=Path);a=p.parse_args();root=a.checkout.resolve()
 def git(*args):return subprocess.check_output(['git','--no-optional-locks',*args],cwd=root)
 head=git('rev-parse','HEAD').decode().strip()
 if len(a.expected_commit)!=40 or head!=a.expected_commit:raise ValueError('Require the exact full approved commit SHA')
 if git('status','--porcelain','--untracked-files=all'):raise ValueError('Preserve changes and use a fresh clean detached worktree')
 entries=[x for x in git('ls-tree','-rz','HEAD').split(b'\0') if x];names=[];oids=[];rows=[]
 for entry in entries:
  meta,path=entry.split(b'\t',1);mode,kind,oid=meta.split()
  if kind!=b'blob':raise ValueError('Unexpected non-file Git entry')
  name=path.decode();p=root/name
  if p.is_symlink():raise ValueError('Unexpected publication symlink: '+name)
  data=p.read_bytes();rows.append(dict(path=name,sha256=digest(data),bytes=len(data)));names.append(name);oids.append(oid)
 result=subprocess.run(['git','cat-file','--batch'],cwd=root,input=b'\n'.join(oids)+b'\n',capture_output=True,check=True).stdout;offset=0
 for row in rows:
  end=result.index(b'\n',offset);size=int(result[offset:end].split()[2]);blob=result[end+1:end+1+size];offset=end+size+2
  if digest(blob)!=row['sha256']:raise ValueError('Raw checkout differs from exact Git blob: '+row['path'])
 record=compare_original(root,names);rows.sort(key=lambda r:r['path'].encode());record.update(head=head,tree=git('rev-parse','HEAD^{tree}').decode().strip(),publication_fingerprint=digest(''.join(r['path']+'\0'+r['sha256']+'\n' for r in rows).encode()),git_blobs_raw_equal=True,native_tests_executed=False,inventory=rows)
 if a.output:
  target=a.output.resolve()
  if target.exists():raise ValueError('Retain existing output; choose a new path')
  if target.is_relative_to(root) and target.relative_to(root).parts[0]!='build':raise ValueError('Write evidence under ignored build or outside source')
  target.parent.mkdir(parents=True,exist_ok=True);target.write_text(json.dumps(record,indent=2)+'\n')
 print(json.dumps({k:v for k,v in record.items() if k!='inventory'},indent=2))
if __name__=='__main__':main()
