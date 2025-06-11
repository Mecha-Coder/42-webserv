#!/usr/bin/env python3

import os
import sys
import html

def generate_list(items):
	html_parts = [
		"<!DOCTYPE html>",
		"<html lang='en'><head><meta charset='utf-8'>",
		"<meta name='viewport' content='width=device-width, initial-scale=1.0'>",
		"<title>Directory Contents</title>",
		"<style>",
		"body { font-family: sans-serif; padding: 2rem; }",
		"ul { list-style: none; padding: 0; max-width: 600px; }",
		"li { display: flex; align-items: center; justify-content: space-between;",
		"     border: 1px solid #ccc; border-radius: 4px; padding: 8px 12px;",
		"     margin-bottom: 10px; }",
		".del-btn { margin-right: 16px; background: #e74c3c; color: white; border: none;",
		"           padding: 4px 10px; border-radius: 4px; cursor: pointer; }",
		".del-btn:hover { background: #c0392b; }",
		".file-label { flex-grow: 1; }",
		"</style>",
		"</head><body>",
		"<h2>Files and Folders</h2>",
		"<ul id='file-list'>"
	]

	for name in sorted(items, key=str.lower):
		safe_name = name.replace('"', '&quot;').replace("'", "\\'")
		escaped_name = html.escape(name)
		html_parts.append(
			f"<li data-name=\"{escaped_name}\">"
			f"<button class='del-btn' onclick=\"deleteItem('{safe_name}')\">✖</button>"
			f"<span class='file-label'>{escaped_name}</span>"
			"</li>"
		)

	html_parts += [
		"</ul>",
		"<script>",
		"async function deleteItem(name) {",
		"  if (!confirm('Delete \"' + name + '\"?')) return;",
		"  try {",
		f"    const res = await fetch('/archive/' + name, {{ method: 'DELETE' }});",
		"    if (res.status === 204) {",
		"      alert('Deleted: ' + name);",
		"      const li = document.querySelector(`li[data-name=\"${name}\"]`);",
		"      if (li) li.remove();",
		"    } else if (res.status === 404) {",
		"      alert('Not found: ' + name);",
		"    } else {",
		"      alert('Error: ' + res.status);",
		"    }",
		"  } catch (err) {",
		"    alert('Request failed: ' + err);",
		"  }",
		"}",
		"</script>",
		"</body></html>"
	]

	return "\n".join(html_parts)

#============================================================================

def makeHeader(content_length: int) -> str:
	return (
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		f"Content-Length: {content_length}\r\n\r\n"
	)

#============================================================================

def main():
	root = os.environ.get("ROOT", "")
	filepath = os.path.join(root, "archive")
	render_list = []

	try:
		filelist = os.listdir(filepath)
		for name in sorted(filelist, key=str.lower):
			if os.path.isdir(os.path.join(filepath, name)):
				render_list.append(name + "/")
			else:
				render_list.append(name)
	except Exception as e:
		print(f"{e}", file=sys.stderr)

	body_str = generate_list(render_list)
	header_str = makeHeader(len(body_str.encode('utf-8')))

	sys.stdout.write(header_str)
	sys.stdout.write(body_str)

#============================================================================

if __name__ == "__main__":
	main()
