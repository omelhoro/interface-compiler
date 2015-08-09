/*eslint-env node*/
const babel = require('babel/register');
const app = require('./parser/es6');
const fs   = require('fs');

const yaml = require('js-yaml');
const beautify = require('js-beautify').js_beautify;

// Get document, or throw exception on error
try {
  var doc = yaml.safeLoad(fs.readFileSync('./interface.yaml', 'utf8'));
  console.log(doc);
} catch (e) {
  console.log(e);
}

var STR = app(doc[0]);

var beaut = beautify(STR, {indent_size: 2});
fs.writeFileSync('./out/out.es6', beaut);
