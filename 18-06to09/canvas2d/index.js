var ctx, canvas;

function translate( x, y ) {
    x = x  + canvas.width * 0.5;
    y = -y + canvas.height * 0.5;
    return { x, y };
}

function init( props ) {
    canvas = props.canvas;
    ctx = canvas.getContext( '2d' );

    var changeList = [ "moveTo", "lineTo", "fillRect", "clearRect" ];
    for( let i = 0; i < changeList.length; i++ ) {
        let name = changeList[ i ];
        ctx[ "old_" + name ] = ctx[ name ];
        ctx[ name ] = function (x, y, w, h ) {
            let coord = translate( x, y );
            ctx[ "old_" + name ]( coord.x, coord.y, w, -h );
        }
    }
    // ctx.translate( 400, 300 );
    
}

const EPLISION = 0.00001;



function drawWithLine( v ) {
    ctx.beginPath();
    
    ctx.moveTo( v[0], v[1] );
    for( let i = 1; i <= v.numItems; i++ ) {
        ctx.lineTo( v[ i * 2 ], v[ i * 2 + 1 ] );
    }
    ctx.stroke();
    
    ctx.closePath();
}

class Circle {
    /**
     * @param {Number} seg segment
     * @param {Number} r   radius
     */
    constructor(seg, r ) {
        this.seg = seg || 100;
        this.r = r || 100;
        this.calcVertex();
    }

    /**
     * @param {Number} seg segment
     * @param {Number} r   radius
     */
    calcVertex() {
        let seg = this.seg;
        let r   = this.r;
        let ox, oy;
        
        let vertex = [];
        vertex.itemSize = 2;

        for( let i = 0; i <= seg; i++ ) {
            ox = Math.cos( 2 * Math.PI * i / seg ) * r;
            oy = Math.sin( 2 * Math.PI * i / seg ) * r;
            vertex.push( ox, oy );
        }

        vertex.numItems = vertex.length / vertex.itemSize;
        this.vertex = vertex;
        return this;
    }

    /**
     * @param {Number} seg segment
     * @param {Number} r   radius
     */
    draw() {
        ctx.strokeStyle = 'green';
        let v = this.vertex;
        drawWithLine( v );
    }
}

class Oval {
    /**
     * @param {Number} seg segment
     * @param {Number} r   radius
     */
    constructor( seg, r, mat2 ) {
        this.seg = seg || 100;
        this.r = r || 100;
        this._mat2 = mat2;
        this.calcVertex();
    }

    get mat2() {
        return this._mat2;
    }

    /**
     * @param {Array<Number>} m
     */
    set mat2( m ) {
        this._mat2 = m;
        this.calcVertex();
    }
    
    calcVertex( interfere ) {
        let ox, oy;
        let seg    = this.seg;
        let r      = this.r;
        let mat2   = this._mat2;
        let vertex = new Circle( seg, r ).vertex;
        
        for( let i = 0; i <= seg; i++ ) {
            ox = vertex[ i * 2 ];
            oy = vertex[ i * 2 + 1 ];
            vertex[ i * 2 ]     = ox * mat2[0] + oy * mat2[1]; // + parseInt( Math.random(1) * 10 );
            vertex[ i * 2 + 1 ] = ox * mat2[2] + oy * mat2[3]; // + parseInt( Math.random(1) * 10 );
            // vertex.push( x, y );
        }
    
        vertex.numItems = vertex.length / vertex.itemSize;
        this._vertex = vertex;
        return this;
    }
    
    draw( mat2 ) {
        ctx.strokeStyle = 'red';
        let v = this._vertex;        
        drawWithLine( v );
    }

    testOval() {
        const radius = 10000;
        const xMax = Math.sqrt( 4 / 3 * radius );
        const xMin = - xMax;
        const seg = 100;
        const segLength = xMax * 2 / seg;
        let x, y;
        let vertex = [];
        vertex.itemSize = 2;
        for( let i = 0; i <= seg; i++ ) {
            x = xMin + segLength * i;
            y = Math.sqrt( radius - 0.75 * x * x ) - 0.5 * x;
            vertex.push( x, y );
        }
        for( let i = 0; i <= seg; i++ ) {
            x = xMax - segLength * i;
            y = - Math.sqrt( radius - 0.75 * x * x ) - 0.5 * x;
            vertex.push( x, y );
        }
        vertex.numItems = vertex.length / vertex.itemSize;
        this._testVertex = vertex;
    }

    testOvalDraw( ) {
        ctx.strokeStyle = 'blue';
        drawWithLine( this._testVertex )
    }
}


function drawCoord() {
    ctx.strokeStyle = "black"
    ctx.beginPath();
    ctx.moveTo( 0, 0 );
    ctx.lineTo( 400, 0 );
    ctx.closePath();
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo( 0, 0 );
    ctx.lineTo( 0, 400 );
    ctx.closePath();
    ctx.stroke();
}

// export {init, drawCoord, Oval, drawWithLine, Circle}