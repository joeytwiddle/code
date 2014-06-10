<Sorella> eval(fs.readdirSync('your/dir').map(function(it){ return fs.readFileSync('your/dir/' + it, 'utf-8') }).join(';'))

async: <Sorella> fs = require('future-fs'); fs.directory.list('your/dir').map(fs.file.read).reduce(join).value(eval)
