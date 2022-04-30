# node-silk

[![npm version](https://img.shields.io/npm/v/node-silk/latest.svg)](https://www.npmjs.com/package/node-silk)
[![dm](https://shields.io/npm/dm/node-silk)](https://www.npmjs.com/package/node-silk)

silk codec bindings for Node.js

**Install:**

```bash
# npm i node-silk
```

**Usage:**

```js
const fs = require("fs")
const { pcm2slk } = require("node-silk")
const pcmbuf = fs.readFileSync("/tmp/test.pcm")
const slkbuf = pcm2slk(pcmbuf)
```
