"use strict"
const fs = require("fs")
const { pcm2slk, slk2pcm } = require("./")

var pcm = fs.readFileSync("D:/konata_test.pcm")
var slk = fs.readFileSync("D:/test.slk")

console.time()
for (var i = 0; i < 1; i++) {
	var silk = pcm2slk(pcm)
}
console.timeEnd()

console.log(silk)
fs.writeFileSync("D:/konata_test.silk", silk)

console.time()
for (var i = 0; i < 1; i++) {
	var pcm = slk2pcm(slk)
}
console.timeEnd()

console.log(pcm)
fs.writeFileSync("D:/test.pcm", pcm)
