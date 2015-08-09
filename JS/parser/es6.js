const fs   = require('fs');

String.prototype.toUnderscore = function() {
  return this.replace(/([A-Z])/g, ($1) => '_' + $1.toLowerCase());
};

function lower(string) {
  return string.charAt(0).toLowerCase() + string.slice(1);
}

function upper(string) {
  return string.charAt(0).toUpperCase() + string.slice(1);
}

function parserNamespace(node, depth) {
  var str =
`${depth.join('.')} = {};
`;
  return str;
}

function parserParams(params, depth, isSending) {
  var str = '';
  for (let par of params) {
    var nm = lower(par.name);

    if (isSending) {
      str += `everbase.rpc.jsonEncoding._conversions['${par.type}'].encode(${nm}, [] ),`;
    } else {
      str += `${nm} /* ${par.type} */,`;
    }
  }

  return str.slice(0, -1);
}

function parserFunction(fns, depth) {
  var out = '';
  for (let fn of fns) {

    // replace dots with double colons for API call
    var target = depth.map(upper).join('::') + `::${fn.name}`;
    fn.params = fn.params || [];
    if (fn.return && fn.return.type instanceof Array) {
      var rightSide = `[resolve, reject, '${fn.return.type[0]}', ['${fn.return.type[1]}']]`;
    } else {
      var rightSide = `[resolve, reject, '${fn.return ? fn.return.type :  ''}', []]`;
    }

    out +=
` ${docFn(fn)}

  ${fn.static ? 'static' : ''} ${lower(fn.name)}(${parserParams(fn.params)}) {

    var message = [
        'call',
        '${target}',
        uuid(),
        [${fn.static ? '' : 'this._handle,'}
        ${parserParams(fn.params, depth, true)}],
    ];

    everbase.webclient.connection.send(message);
    return new Promise((resolve, reject) => everbase.webclient.processes[message[2]] = ${rightSide});

  }
`;
  }

  return out;
}

// a helping fn to generate docs
function docFn(node) {

  // doc.more is optional
  var str =
`/*
  ${node.doc.brief}
  ${node.doc.more || ''}
*/`;

  return str;
}

function parserStruct(fields, depth) {
  var out = '';
  for (let field of fields) {
    var nm = lower(field.name);
    var str =
`
get ${nm}() {
  return this._${nm};
}
set ${nm}(a) {
  this._${nm} = a;
}
`;
    out += str;
  }

  return out;
}

function parserEvent(events, depth) {
  var out = '';
  var evbNm = depth.map(upper).join('::');
  var objPath = depth.join('.');
  for (let event of events) {
    var nm = lower(event.name);
    var clsPath = objPath + '.' + nm;
    var clsName = clsPath.replace(/\./g, '');
    var TYPE_NAME = evbNm + '::' + event.name;
    var str =
    `
class ${clsName} {

  constructor() {
    this.TYPE_NAME = '${TYPE_NAME}';
  }

}

${clsPath} = ${clsName};
${clsPath}.TYPE_NAME = '${TYPE_NAME}';
everbase.rpc.jsonEncoding._conversions['${evbNm}::${event.name}'] = eventDecode(${objPath}.${nm} ,${JSON.stringify(event.values)})
`;

    out += str;
  }

  return out;
}

function parserConstants(consts, depth) {
  var objPath = depth.join('.');
  var out = '';
  for (let con of consts) {
    var enumName = lower(con.name).toUnderscore().toUpperCase();
    var str = `
${objPath}.${enumName} = '${con.value}';
    `;

    out += str;
  }

  return out;
}

function parserClass(node, depth) {
  var path = depth.join('');
  var evbNm = depth.map(upper).join('::');
  if (node.type === 'struct') {
    var instant = `everbase.rpc.jsonEncoding._conversions['${evbNm}'] = structInstant(${depth.join('.')}, ${JSON.stringify(node.fields)});`;
  } else {
    var instant = `everbase.rpc.jsonEncoding._conversions['${evbNm}'] = classInstant(${depth.join('.')});`;
  }

  var str = `
${docFn(node)}

class ${path} {

  constructor(handle) {
    this._handle = typeof handle !== 'undefined' ? handle : null;
  }

  ${node.operations ? parserFunction(node.operations, depth) : ''}
  ${node.fields ? parserStruct(node.fields, depth) : ''}

};
${depth.join('.')} = ${path};
${instant};
${node.events ? parserEvent(node.events, depth) : ''}
${node.constants ? parserConstants(node.constants, depth) : ''}
`;
  return str;
}

function parserPrimitive(node, depth) {
  var evbNm = depth.map(upper).join('::');
  var str =
`everbase.rpc.jsonEncoding._conversions['${evbNm}'] = {encode: (value) => value, decode: (value) => value,};
//everbase.rpc.jsonEncoding._conversions.${node.name} = {encode: (value) => value, decode: (value) => value,};

`;

  return str;
}

function parserEnum(node, depth) {
  var nm = depth.map(lower).join('.');
  var evbNm = depth.map(upper).join('::');
  var str = `${nm} = {};`;
  for (let val of node.values) {
    console.log(val);
    var enumName = lower(val.name).toUnderscore().toUpperCase();
    str +=
`
// ${val.doc.brief}
${nm}.${enumName} = ${val.value};
`;
  }

  str += `
everbase.rpc.jsonEncoding._conversions['${evbNm}'] = {
    encode: (a) => a,
    decode: (a) => a,
}
`;
  return str;

}

function parser(ty) {
  return (...args) => {
    switch (ty) {
      case 'struct':
      case 'class':
        var out = parserClass.apply(null, args);
        break;
      case 'namespace':
        var out = parserNamespace.apply(null, args);
        break;
      case 'enum':
        var out = parserEnum.apply(null, args);
        break;
      default:
        console.log('Not implemented', ty);
        var out = '';
    }
    return out;
  };
}

var typeParser = {
  primitive: parser('primitive'),
  namespace: parser('namespace'),
  struct: parser('struct'),
  enum: parser('enum'),
  class: parser('class'),
};

function traverse(node, depth) {
  node.members = node.members || [];
  for (let mem of node.members) {
    try {
      var fn = typeParser[mem.type];
    } catch (e) {
      console.log('No parser for', mem.type, e);
      return;
    }

    var memName = ['class', 'enum', 'primitive'].indexOf(mem.type) > -1 ? mem.name : lower(mem.name);
    var curNode = depth.concat([memName]);
    var out = fn(mem, curNode);
    STR += out;
    traverse(mem, curNode);
    console.log('position:', curNode);
  }
}

var STR = `
(function(){
function lower(string) {
  return string.charAt(0).toLowerCase() + string.slice(1);
}

function upper(string) {
  return string.charAt(0).toUpperCase() + string.slice(1);
}

window.everbase = {};

`;

var classInstant = `
function classInstant(cls) {
  return {
    encode: function (classObj) {
        return classObj._handle;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) {
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new cls(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
  }
}

function structInstant(cls, fields) {
  return {
    encode: function (value) {
        var result = new cls();
        for (let field of fields) {
          var ty = field.type instanceof Array ? field.type : field.type ;
          result[lower(field.name)] = everbase.rpc.jsonEncoding._conversions[ty].encode(value[field.name], field.type[1]);
        }
        return result;
    },
    decode: function(handle) {
        if (handle in everbase.rpc.jsonEncoding._handles) {
            return everbase.rpc.jsonEncoding._handles[handle]
        }
        var result = new cls(handle);
        everbase.rpc.jsonEncoding._handles[handle] = result;
        return result;
    }
  }
}


everbase.rpc.jsonEncoding._handles = everbase.rpc.jsonEncoding._handles || {};
function eventDecode(cls, types) {
    return {
    decode: (eventValues) => {
        var JsEvent = new cls();
        for (var i = 0; i < eventValues.length; i++) {
          var item = eventValues[i];
          var ty = types[i];
          JsEvent[lower(ty.name)] = everbase.rpc.jsonEncoding._conversions[ty.type].decode(item, []);
        }
        return JsEvent
    }
  }
}
`;

export default function createJS(doc) {
  var uuid = fs.readFileSync('./includes/Uuid.js');
  var evman = fs.readFileSync('./includes/EventManager.js');
  var jscor = fs.readFileSync('./includes/JsonEncodingCore.js');
  var ws = fs.readFileSync('./includes/WebSocket.js');
  var wcc = fs.readFileSync('./includes/WebClientCore.js');
  STR += uuid + evman + jscor + ws + wcc + classInstant;
  traverse(doc, ['everbase']);
  STR += `})()`;
  return STR;
}
