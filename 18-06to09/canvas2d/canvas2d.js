// import {Circle, Oval, init, drawCoord} from './index'

var canvas = document.getElementById('canvas');
var ctx = canvas.getContext('2d');

init( {canvas, ctx} )

const seg = 100;
const radius = 100;

var pA = document.getElementById( 'paramA' );
var pavalue = document.getElementById( 'pavalue' );

pA.onchange = function() {
    oval.mat2[ 0 ] = pA.value;
    oval.mat2[ 3 ] = pA.value;
    pavalue.innerHTML = pA.value;
    draw();
}

var pB = document.getElementById( 'paramB' );
var pbvalue = document.getElementById( 'pbvalue' );
pB.onchange = function() {
    oval.mat2[ 1 ] = - pB.value;
    oval.mat2[ 2 ] = - pB.value;
    pbvalue.innerHTML = pB.value;
    draw();
}

// var pC = document.getElementById( 'paramC' );
// pC.onchange = function(value) {
//     oval.mat2[ 3 ] = pC.value;
//     draw();

// }
let mat2 = [
    pA.value,  -pB.value/2, 
    -pB.value/2,  pA.value
];

var circle = new Circle( seg, radius );
var oval = new Oval( seg, radius, mat2 );
oval.testOval();

var add = true;
function draw() {
    ctx.clearRect( -canvas.width * 0.5, -canvas.height * 0.5, canvas.width, canvas.height );
    // ctx.fillRect( -2, -2, 50, 50 );
    drawCoord();
    /*
    if( add === true ) {
        mat2[1] += 0.01;
        mat2[2] += 0.01;
    }
    else {
        mat2[1] -= 0.01;
        mat2[2] -= 0.01;
    }
    if( mat2[1] > B/2 ) {
        add = false;
    }
    else if(  mat2[2] < -B/2 ) {
        add = true;
    }
    */
    circle.draw();
    oval.mat2 = mat2;
    oval.draw();
    oval.testOvalDraw();
    // requestAnimationFrame( draw );
}
draw();

// export {circle, oval}
// var timer = setInterval( draw, 1000 );