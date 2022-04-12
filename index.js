"use strict"

const path = require('path')
const binary = require('@mapbox/node-pre-gyp')
const binding_path = binary.find(path.resolve(path.join(__dirname, './package.json')))

const { silkEncode, silkDecode } = require(binding_path)

const err = new Error("param must be a buffer")

function pcm2slk(data) {
	if (!Buffer.isBuffer(data))
		throw err
	const arr = []
	silkEncode(data, chunk => arr.push(chunk))
	return Buffer.concat(arr)
}

function slk2pcm(data) {
	if (!Buffer.isBuffer(data))
		throw err
	const arr = []
	silkDecode(data, chunk => arr.push(chunk))
	return Buffer.concat(arr)
}

module.exports = {
	pcm2slk, slk2pcm
}
