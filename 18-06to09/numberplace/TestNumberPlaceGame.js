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


QUnit.test("getRandomValueArray", (assert) => {
    let values = Utils.getRandomValueArray();
    let result = Utils.isGridsValueValid(values);
    assert.ok( result, `All array values are expected to not equal, array is ${values}` );
    let exclude = [1, 3, 6, 8];
    let remainValues = Utils.getRandomValueArray(exclude);
    result = Utils.isGridsValueValid(remainValues);
    assert.equal( remainValues.length, 5, "Count of emained elements of value array is right" )
    assert.equal( result, false, `All array values are expected to not equal, array is ${remainValues}` );
    exclude = [1, 3, 3, 6, 8];
    remainValues = Utils.getRandomValueArray(exclude);
    result = Utils.isGridsValueValid(remainValues);
    assert.equal( remainValues.length, 5, "Count of emained elements of value array is right" )
    assert.equal( result, false, `All array values are expected to not equal, array is ${remainValues}` );
});

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

QUnit.module("Class Board", {
    before: () => {
        this.board = new Board();
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
            assert.equal(grid.row, row, `Grids (${row}, ${i}) row is right!`);
            assert.equal(grid.col, i,   `Grids (${row}, ${i}) col is right!`);
        }

        col = Math.floor( Math.random() * 9 );
        grids = this.board.getGrids({mode: "column", col: col});
        for (let i = 0; i < 9; i++) {
            const grid = grids[i];
            assert.equal(grid.row, i,   `Grids (${i}, ${col}) row is right!`);
            assert.equal(grid.col, col, `Grids (${i}, ${col}) col is right!`);
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