import { execFileSync } from 'node:child_process';
import { cp, mkdir, mkdtemp, readFile, rm, writeFile } from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { dirname, join, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = resolve(dirname(fileURLToPath(import.meta.url)), '..');
const source = resolve(process.argv[2] || join(root, '../../demos3idkman/sf-voter-guides'));
const output = join(root, 'dist/sf-voter-guides');
const temporary = await mkdtemp(join(tmpdir(), 'lizzie-voter-guides-'));
const navigation = '<nav id="site-pages" aria-label="Site pages" style="display:flex;flex-wrap:wrap;justify-content:center;gap:.5rem 1.25rem;padding:1rem;font:inherit"><a href="/">Home</a><a href="/sf-voter-guides/">SF voter guides</a><a href="/resume/">Résumé</a><a href="/thesis/">Thesis</a></nav>';

try {
  execFileSync('npm', ['run', 'build', '--', '--base=/sf-voter-guides/', '--outDir=' + temporary, '--emptyOutDir'], { cwd: source, stdio: 'inherit' });
  const html = await readFile(join(temporary, 'index.html'), 'utf8');
  if (!html.includes('<div id="root"></div>')) throw new Error('Voter guide app root was not found.');
  await writeFile(join(temporary, 'index.html'), html.replace('<div id="root"></div>', navigation + '\n    <div id="root"></div>'));
  const headers = (await readFile(join(temporary, '_headers'), 'utf8')).replace(/^\/\*/, '/sf-voter-guides/*');
  await rm(join(temporary, '_headers'));
  await mkdir(dirname(output), { recursive: true });
  await rm(output, { recursive: true, force: true });
  await cp(temporary, output, { recursive: true });
  const headersFile = join(root, 'dist/_headers');
  let existing = '';
  try { existing = await readFile(headersFile, 'utf8'); } catch (error) { if (error.code !== 'ENOENT') throw error; }
  if (!existing.includes('/sf-voter-guides/*')) await writeFile(headersFile, existing + (existing ? '\n' : '') + headers);
  const homepageFile = join(root, 'dist/index.html');
  const homepage = await readFile(homepageFile, 'utf8');
  if (!homepage.includes('id="site-pages"')) {
    if (!homepage.includes('<div id="content">')) throw new Error('Personal homepage content container was not found.');
    await writeFile(homepageFile, homepage.replace('<div id="content">', '<div id="content">\n' + navigation));
  }
  console.log('Built /sf-voter-guides/ and linked the public site pages.');
} finally {
  await rm(temporary, { recursive: true, force: true });
}
