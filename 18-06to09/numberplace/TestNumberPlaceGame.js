/*=============== Utils Test ===============*/
QUnit.module("Class Utils", {
    before: () => {
        this.board = new Board();
        for(let i = 0; i < 9; i++) {
            this.board.grids[i].setValue(i+1);
        }
    }
    // beforeEach: assert => {

    // },
    // afterEach: assert => {

    // }
});

QUnit.test("isGridsValid", (assert) => {
    let grids = new Array(9);
    for (let i = 0; i < 9; i++) {
        grids[i] = (i+1);
    }
    let valid = Utils.isGridsValueValid(grids);
    assert.equal( valid, true, "grids value is valid!");

    let i = Math.floor( Math.random() * 9 )
    grids[i] = 10;
    valid = Utils.isGridsValueValid(grids);
    assert.equal( valid, false, "grids value is not valid!");
});

QUnit.test("randomArray", assert => {
    let basic  = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    let values = Utils.randomArray( basic )
    assert.equal(values.length, 9, `Randomized the input array, result: ${values}`);
});

QUnit.test("distinctArray", assert => {
    let basic, values, result;
    result = true;
    basic  = [1, 3, 1, 2, 3, 6, 8];
    values = Utils.distinctArray( basic );
    values.sort( (a, b) => {
        return a > b;
    })
    for(let i = 0; i < values.length - 1; i++) {
        result &= (values[i] < values[i+1]);
    }
    assert.ok( result, `Array is distincted: ${values}` )

    basic  = [1, 2, 3, 6, 8];
    values = Utils.distinctArray( basic );
    result = true;
    values.sort( (a, b) => {
        return a > b;
    })
    for(let i = 0; i < values.length - 1; i++) {
        result &= (values[i] < values[i+1]);
    }
    assert.ok( result, `Array is distincted: ${values}` )
});

QUnit.test("getRandomValue", (assert) => {
    let values = Utils.getRandomValue();
    let result = Utils.isGridsValueValid(values);
    assert.ok( result, `All array values are expected to not equal, array is ${values}` );
    
    let exclude = [1, 3, 6, 8];
    let remainValues = Utils.getRandomValue({exclude});
    result = Utils.isGridsValueValid(remainValues);
    assert.equal( remainValues.length, 5, "Count of emained elements of value array is right" )
    assert.equal( result, false, `All array values are expected to not equal, array is ${remainValues}` );

    exclude = [1, 3, 3, 6, 8];
    remainValues = Utils.getRandomValue({exclude});
    result = Utils.isGridsValueValid(remainValues);
    assert.equal( remainValues.length, 5, "Count of emained elements of value array is right" );
    assert.equal( result, false, `All array values are expected to not equal, array is ${remainValues}` );
});
/*=============== Utils Test ===============*/

/*=============== Grid Test ===============*/
QUnit.module("Class Grid", {
    before: () => {
        this.grid = new Grid(0, 0);
        this.times = 10;
    }
});

QUnit.test("isRight", assert => {
    let uvalue = 0;
    let value  = 0;
    
    for(let i = 0; i < this.times; i++) {
        uvalue = Math.floor( Math.random() * 9 );
        value  = Math.floor( Math.random() * 9 );
        this.grid.setValue(value);
        this.grid.placeValue(uvalue);
        assert.equal(this.grid.value,     value,            `Set Grid Value(${value}) is OK`);
        assert.equal(this.grid.userValue, uvalue,           `Set Grid userValue(${uvalue}) is OK`);
        assert.equal(this.grid.isRight(), uvalue === value, `User Place Value (${uvalue} === ${value}) set Right`);
    }
});

QUnit.test("isValidValue", assert => {
    let v = 0;
    let valid;
    let number = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    for(let i = 0; i < this.times*3; i++) {
        v = Math.floor( Math.random() * this.times*3 );
        valid = number.includes(v);
        assert.equal( Grid.isValidValue(v), valid, `Random Value ${v} valid: ${valid}`);
    }
});

QUnit.test("blockBelonged", assert => {
    let bRow, bCol, rowOff, colOff, grid, block, result;
    for(let i = 0; i < this.times; i++) {
        bRow   = Math.floor( Math.random() * 3 );
        bCol   = Math.floor( Math.random() * 3 );
        rowOff = Math.floor( Math.random() * 3 );
        colOff = Math.floor( Math.random() * 3 );
        grid   = new Grid(bRow*3 + rowOff, bCol*3 + colOff);
        block  = Grid.blockBelonged(grid);
        result = ( block.row === bRow ) && ( block.col == bCol );
        assert.ok( result, `Get Right Block: ( ${block.row} === ${bRow} ) && ( ${block.col} == ${bCol} )`);
    }
});
/*=============== Grid Test ===============*/

/*=============== Board Test ===============*/
QUnit.module("Class Board", {
    before: () => {
        this.board = new Board();
        this.board.init();
        this.times = 10;
    }
})

QUnit.test("getRowGrids", assert => {
    let row, grids;
    for(let i = 0; i < this.times; i++) {
        row   = Math.floor( Math.random() * 9 );
        grids = this.board.getRowGrids(row);
        for (let i = 0; i < 9; i++) {
            const grid = grids[i];
            assert.equal(grid.row, row, `Grids (${row}, ${i}) row is right!`);
            assert.equal(grid.col, i,   `Grids (${row}, ${i}) col is right!`);
        }
    }
});

QUnit.test("getColumnGrids", assert => {
    let col, grids;
    for(let i = 0; i < this.times; i++) {
        col   = Math.floor( Math.random() * 9 );
        grids = this.board.getColumnGrids(col);
        for (let i = 0; i < 9; i++) {
            const grid = grids[i];
            assert.equal(grid.row, i,   `Grids (${i}, ${col}) row is right!`);
            assert.equal(grid.col, col, `Grids (${i}, ${col}) col is right!`);
        }
    }
});

QUnit.test("getBlockGrids", assert => {
    let row, col, grids;
    let right, block;
    for(let i = 0; i < this.times; i++) {
        row   = Math.floor( Math.random() * 3 );
        col   = Math.floor( Math.random() * 3 );
        grids = this.board.getBlockGrids(row, col);
        assert.equal(grids.length, 9, `Block(${row}, ${col}) size is right!`);
        grids.forEach(grid => {
            assert.ok(grid instanceof Object, `Each ${grid} is an Object` )
            block = Grid.blockBelonged(grid)
            right = ( block.row === row ) && ( block.col === col );
            assert.ok(right, "Block is Right");
        });
    }
});

QUnit.test("getGrids", assert => {
    let row, col, grids;
    for(let i = 0; i < this.times; i++) {
        row   = Math.floor( Math.random() * 9 );
        grids = this.board.getGrids({mode: "row", row: row});
        for (let i = 0; i < 9; i++) {
            const grid = grids[i];
            assert.ok( ( grid.row === row && grid.col === i ), `Grids (${row}, ${i}) row is right!`);
        }

        col = Math.floor( Math.random() * 9 );
        grids = this.board.getGrids({mode: "column", col: col});
        for (let i = 0; i < 9; i++) {
            const grid = grids[i];
            assert.ok( ( grid.row === i && grid.col === col ), `Grids (${i}, ${col}) col is right!`);
        }
    }
    for(let i = 0; i < this.times; i++) {
        row   = Math.floor( Math.random() * 3 );
        col   = Math.floor( Math.random() * 3 );
        grids = this.board.getGrids({mode: "block", row: row, col: col});
        assert.equal(grids.length, 9, "Block size is right!");
        grids.forEach(grid => {
            assert.ok(grid instanceof Object, `Each ${grid} is an Object` )
        });
    }

    grids = this.board.getGrids({mode: "none"});
    assert.equal(grids.length, 0, "Get Empty Grids");
});

QUnit.test("getPartialGrids", assert => {
    let rowStart, colStart, rowEnd, colEnd;
    let pGrids, rGrids, cGrids, bGrids, result;
    rowStart = 0, colStart = 0; 
    rowEnd   = 1, colEnd   = 9;
    pGrids = this.board.getPartialGrids({rowStart, colStart, rowEnd, colEnd});
    rGrids = this.board.getRowGrids(rowStart);
    result = true;
    pGrids.forEach( (grid, index) => {
        result &= (grid === rGrids[index]);
    });
    assert.ok( result, `Partial Grids is Right`);
    rowStart = 0, colStart = 3;
    rowEnd   = 9, colEnd   = 4;
    pGrids = this.board.getPartialGrids({rowStart, colStart, rowEnd, colEnd});
    cGrids = this.board.getColumnGrids(colStart);
    result = true;
    pGrids.forEach( (grid, index) => {
        result &= (grid === cGrids[index]);
    });
    assert.ok( result, `Partial Grids is Right`);

    rowStart = 3, colStart = 3;
    rowEnd   = 6, colEnd   = 6;
    pGrids = this.board.getPartialGrids({rowStart, colStart, rowEnd, colEnd});
    bGrids = this.board.getBlockGrids(1, 1);
    result = true;
    pGrids.forEach( (grid, index) => {
        result &= (grid === bGrids[index]);
    });
    assert.ok( result, `Partial Grids is Right`);
});

QUnit.test("getUsedValueArrayAt", assert => {
    let row, col;
    for(let i = 0; i < this.times; i++) {
        row = Math.floor( Math.random() * 9 );
        col = Math.floor( Math.random() * 9 );
        let usedValue = this.board.getUsedValueArrayAt({row, col});
        assert.equal( usedValue.length, 9, "Used Value Array length is right");
    }
});

/*=============== Board Test ===============*/

QUnit.module("Performance", {
    before: () => {
        this.times = 100;
    }
});

QUnit.test("Board Init", assert => {
    let valid;
    let validCount = 0;
    console.time("init board")
    for (let i = 0; i < this.times; i++) {
        let g = new Game(0);
        Utils.printAll(g.board);
        valid = g.board.isValid();
        if( valid )
            validCount++;

    }
    console.timeEnd("init board")
    assert.equal(validCount, this.times, `Initialized new Valid Games ${validCount} times`);
});