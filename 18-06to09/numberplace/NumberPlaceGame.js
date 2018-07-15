class Grid {
    /**
     * 
     * @param {Number} row 
     * @param {Number} col 
     */
    constructor( row, col ) {
        this.row = row;
        this.col = col;
        // this.blockRow = Math.floor( row / 3 );
        // this.blockCol = Math.floor( col / 3 );
        this.visibility = true;
        this.value = 0;
        this.userValue = 0;
    }

    /**
     *
     * @param {In} value
     */
    setValue( value ) {
        this.value = value;
    }

    placeValue( value ) {
        this.userValue = value;
    }

    isRight() {
        return this.value === this.userValue;
    }

    /**
     *
     * @param {Boolean} v
     */
    setVisibility( v ) {
        this.visibility = v;
    }

    isVisible() {
        return this.visibility;
    }

    /**
     * 
     * @param {Number} v 
     */
    static isValidValue(v) {
        if( v > 0 && v < 10 ) {
            return true;
        }
        return false;
    }

    /**
     * 
     * @param {{row: Number, col: number}} grid 
     * @returns {{row:Number, col: Number}}
     */
    static blockBelonged(grid) {
        let block = {};
        block.row = Math.floor( grid.row / 3 );
        block.col = Math.floor( grid.col / 3 );
        return block;
    }
}

class Utils {
    /**
     * 
     * @param   {Array<Number>} exclude 
     * @returns {Array<Number>}
     */
    static getRandomValueArray(exclude) {
        let baseArr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
        let exCount = 0;
        if( exclude && exclude.length > 0 ) {
            baseArr.forEach( (element, index) => {
                if( exclude.includes(element) ) {
                    baseArr[index] = 100;  // 移除要排除的数字
                    exCount++;
                }
            });
            baseArr.sort( (a, b) => {
                return a > b;  // 顺序排列
            });
            for (let i = 0; i < exCount; i++) {
                baseArr.pop();
            }
        }

       let arr = new Array(9 - exCount);
       let randomIndex;
       for(let i = 0; i < arr.length; i++) {
           randomIndex = Math.floor( Math.random() * (9 - exCount) );
           arr[i] = baseArr[randomIndex];
           baseArr[randomIndex] = baseArr[ 9 - 1 - exCount ];
           baseArr[ 9 - 1 - exCount ] = 0;
           exCount++;
       }
       return arr;
    }

    /**
     * 
     * @param {Array<Number>} grids 
     *      
     * @returns {Boolean}
     */
    static isGridsValueValid( gridsValue ) {
        let sum = 0;

        for( let i = 0; i < 9; i++ ) {
            sum += gridsValue[i];
        }

        if( sum == 45 ) {
            return true;
        }
        return false;
    }
}

class Board {

    constructor() {
        this.grids = new Array( 81 );

        for( let i = 0; i < 9; i++ ) {
            for( let j = 0; j < 9; j++ ) {
                this.grids[i * 9 + j] = new Grid( i, j );
            }
        }
    }

    init() {
        // populate the first row
        let row0 = this.getRowGrids(0);
        let randomArray = this.getRandomValueArray([]);
        randomArray.forEach( (element, index ) => {
            row0[index].setValue( element );
        });

        this.printAll();
 
        let validValue;
        for(let i = 1; i < 9; i++) {
            for(let j = 0; j < 9; j++) {
                validValue = this.getRandomValidValue({row: i, col: j});
                this.grids[i*9+j].setValue( validValue );
                this.printAll();
            }
        }
    }

    /**
     * 
     * @param {{row: Number, col: Number}} pos 
     * @return Number
     */
    getRandomValidValue(pos) {
        let used = this.getUsedValueArrayAt(pos);
        let valueArray = this.getRandomValueArray(used);
        return valueArray[0] === undefined ? 0 : valueArray[0];
    }

    /**
     * 
     * @param {{mode: String, row: Number, col: Number}} cond 
     *      mode can be row, column or block
     * @returns Array
     */
    getUsedValueArray( cond ) {
        let mode = cond.mode;
        let grids;
        switch (mode) {
            case "row":
            grids = this.getRowGrids( cond.row );
            break;
            case "column":
            grids = this.getColumnGrids( cond.col );
            break;
            case "block":
            grids = this.getBlockGrids( cond.row, cond.col );
            break;
            default:
            grids = new Array();
            break;
        }
        let arr = new Array();
        grids.forEach( element => {
            if( Grid.isValidValue( element.value ) )
                arr.push( element.value )
        });
        return arr;
    }

    /**
     * 
     * @param {{row: Number, col: Number}} pos 
     * @returns Array<Number>
     */
    getUsedValueArrayAt(pos) {
        let row = pos.row;
        let col = pos.col;
        let used = new Array();
        let block = Grid.blockBelonged(pos);
        let rowGrids = this.getUsedValueArray( {mode: "row", row: row} );
        let colGrids = this.getUsedValueArray( {mode: "column", col: col});
        let blockGrids = this.getUsedValueArray( {mode: "block", row: block.row, col: block.col} );
        used.push.apply( used, rowGrids );
        used.push.apply( used, colGrids );
        used.push.apply( used, blockGrids );
        let result = used.sort().reduce( (pre, current) => {
            if( pre.length == 0 || pre[pre.length-1] != current ) {
                pre.push( current );
            }
            return pre;
        }, []);
        console.log( row + "  " + "  " + col + "  ", result )
        return result;
    }

    /**
     *
     * @param {Number} row
     * @return {Array<Grid>}
     */
    getRowGrids( row ) {
        let rowArray = new Array( 9 );

        for( let i = 0; i < 9; i++ ) {
            rowArray[i] = this.grids[i + row * 9];
        }

        return rowArray;
    }

    /**
     * 
     * @param {Number} col 
     * @return {Array<Grid>}
     */
    getColumnGrids( col ) {
        let colArray = new Array( 9 );

        for( let i = 0; i < 9; i++ ) {
            colArray[i] = this.grids[i * 9 + col];
        }

        return colArray;
    }

    /**
     * 
     * @param {Number} row 
     * @param {Number} col 
     * @return {Array<Grid>}
     */
    getBlockGrids( row, col ) {
        let block = new Array( 9 );

        for( let i = 0; i < 3; i++ ) {
            for( let j = 0; j < 3; j++ ) {
                block[i * 3 + j] = this.grids[ ( row ) * 27 + col * 3 + i * 9 + j ];
            }
        }

        return block;
    }

    /**
     * row range: [rowStart, rowEnd) , rowStart is INCLUDED while rowEnd is not
     * col range: [colStart, colEnd)
     * @param {{rowStart: Number, colStart: Number, rowEnd: Number, colEnd: Number}} area 
     * @return {Array<Grid>}
     */
    getPartialGrids(area) {
        let size = (area.rowEnd - area.rowStart) * (area.colEnd - area.colStart);
        if( size <= 0) {
            return new Array();
        }
        let part = new Array();

        for(let i = area.rowStart; i < area.rowEnd; i++) {
            for( let j = area.colStart; j < area.colEnd; j++ ) {
                part.push( this.grids[i*9+j] );
            }
        }
        return part;
    }

    /**
     * 
     * @param {{mode: String, row:Number, col: Number}} cond 
     */
    getGrids(cond) {
        let mode = cond.mode;
        let grids;
        switch (mode) {
            case "row":
            grids = this.getRowGrids( cond.row );
            break;
            case "column":
            grids = this.getColumnGrids( cond.col );
            break;
            case "block":
            grids = this.getBlockGrids( cond.row, cond.col );
            break;
            default: 
            grids = new Array();
            break;
        }
        return grids;
    }

    /**
     * 
     * @param {Number} row 
     * @param {Number} col 
     * @param {Number} num 
     */
    setNumberAt( row, col, num ) {
        if(     row < 0 || row > 8
            ||  col < 0 || col > 8
            ||  num < 0 || num > 9 ) {
            console.log( "Position or Number is invalid." )
            return;
        }

        let grid = this.grids[row * 9 + col];
        grid.placeValue( num );
    }

    printAll() {
        let toPrint = "";

        for( let i = 0; i < 9; i++ ) {
            for( let j = 0; j < 9; j++ ) {
                toPrint += this.grids[i * 9 + j].value + " ";
            }

            toPrint += "\n";
        }

        console.log( toPrint );
    }
}

Board.width  = 9;
Board.height = 9;

class Game {
    constructor( difficuty ) {
        this.board = new Board();
        this.board.init();
        let board = this.board;
        board.setNumberAt( 1, 2, 8 );
        board.setNumberAt( 4, 0, 4 );
        board.setNumberAt( 4, 1, 2 );
        board.setNumberAt( 4, 2, 3 );
        board.setNumberAt( 4, 3, 1 );
        board.setNumberAt( 4, 4, 5 );
        board.setNumberAt( 4, 5, 6 );
        board.setNumberAt( 4, 6, 9 );
        board.setNumberAt( 4, 7, 7 );
        board.setNumberAt( 4, 8, 8 );
        board.printAll();
    }
}

Game.DifficutyEasy = 0;
Game.DifficutyNormal = 0;
Game.DifficutyHard = 0;

// let g = new Game(Game.DifficutyEasy);
// export {Game, Board, Grid}

