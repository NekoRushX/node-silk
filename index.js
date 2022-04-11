"use strict"
const { silkEncode, silkDecode } = require("./lib/binding/node-v83-win32-x64/node-silk.node")
const fs = require("fs")

// /**
//  * @param {Buffer} data 
//  * @returns {Buffer}
//  */
// function encode(data) {
// 	return silkEncode(data)
// }

// /**
//  * @param {Buffer} data 
//  * @returns {Buffer}
//  */
// function decode(data) {
	
// }

// module.exports = require("./lib/binding/node-v83-win32-x64/node-silk.node")
var pcm = fs.readFileSync("D:/konata_test.pcm")
console.log(silkEncode, pcm)
var slk = silkEncode(pcm, d => {
	console.log(1)
	console.log(d)
})
// console.log(1)
console.log(slk)
