const fs =  require('fs');
const exec = require('child_process').exec;

const TO_WATCH = __dirname + '/src';

// avoid multi invoke 
var last = Date.now();
function onChange(eventType, filename) {
    let nd = Date.now();
    if( nd - last > 100 && filename ) {
        // console.log( filename );
        exec("yarn build", (err, stdout, stderr) => {
            console.log( stdout );
        });
    }
    last = nd;
};

var watcher = [];
watcher[0] = fs.watch( TO_WATCH, { recursive: true } );
watcher[0].on( 'change', onChange );

// on linux watch recursive option is invalid
fs.readdir( TO_WATCH, { withFileTypes: true }, (err, files) => {
    if( err ) {
        throw err;
    }

    for(let i = 0; i < files.length; i++ ) {
        let file = files[i]
        // file is just the name of certain file, 
        // we have to prepend "src/" or the absolute path
        fs.stat( TO_WATCH + "/" + file, (err, stats) => {  
            if( err ) {
                throw err;
            }
            
            if( stats && stats.isDirectory() ) {

                let w = watcher.length;
                watcher[w] = fs.watch( TO_WATCH + "/" + file );
                watcher[w].on( 'change', onChange );
            }
        })
    }
} );