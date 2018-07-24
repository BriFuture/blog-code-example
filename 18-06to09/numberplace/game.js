//@ sourceURL=game.js
class TimeUtil {
    constructor() {
        this.tLabel = document.getElementById("time-label");
        this.startTime = new Date();
        this.started = false;
    }
    
    startTimer() {
        let resultLabel = document.getElementById("result-label");
        resultLabel.innerHTML = "";
        if( !this.started ) {
            this.startTime = new Date();
            this.timerId = setInterval(() => {this.displayTime()}, 500);
            this.started = true;
        }
    }
    
    displayTime() {
        let now = new Date();
        let h = now.getHours()   - this.startTime.getHours();
        let m = now.getMinutes() - this.startTime.getMinutes();
        let s = now.getSeconds() - this.startTime.getSeconds();
        if( s < 0 ) {
            m --;
            s += 60;
        }
        if( m < 0 ) {
            h--;
            m += 60;
        }
        h = this.addZero(h);
        m = this.addZero(m);
        s = this.addZero(s);
        this.tLabel.innerHTML = h + ":" + m + ":" + s ;
    }

    addZero(i) {
        if( i < 10 ) {
            i = "0" + i;
        }
        return i;
    }

    stop() {
        this.started = false;
        clearInterval(this.timerId);
        this.tLabel.innerHTML = "00:00:00";
    }
}

let gamediv = document.getElementById("gamediv");
let g;
let gameDifficuty = Game.DifficutyEasy;
let tu = new TimeUtil;
let inputs = new Array();

function genBoard() {
    tu.stop();
    let tBoard;
    tBoard = document.getElementById("board");
    if( tBoard ) {
        tBoard.remove();
    }
    tBoard = document.createElement("table");
    tBoard.setAttribute("align", "center");
    tBoard.setAttribute("id", "board");
    tBoard.setAttribute("class", "board");
    tBoard.setAttribute("boarder", "1");
    g = new Game( gameDifficuty );
    let tr, td, grid, value;
    let ginput;
    for(let i = 0; i < 9; i++) {
        tr = document.createElement("tr");
        for(let j = 0; j < 9; j++) {
            td = document.createElement("td");
            value = g.getValueAt(new Number(i), new Number(j));
            td.setAttribute("class", "grid-show");
            if( value ) {
                td.innerHTML = value;
            }
            else {
                ginput = document.createElement("input");
                inputs.push(ginput);
                ginput.setAttribute("id", `grid-${i}-${j}`);
                ginput.setAttribute("class", "grid-input");
                ginput.setAttribute("type", "text");
                ginput.setAttribute("onkeyup", "placeGrid(this)");
                td.appendChild(ginput);
            }
            tr.appendChild(td);
        }
        tBoard.appendChild(tr);
    }

    gamediv.appendChild(tBoard);
}

function placeGrid(t) {
    let value = new Number(t.value);
    if( isNaN(value ) || value < 1 || value > 9 ) {
        t.value = ""
        value = ""
    }
    let gid = t.id;
    let rowIndex = gid.indexOf('-')+1;
    let row = new Number( gid.substr(rowIndex, 1) );
    let colIndex = gid.indexOf('-', rowIndex)+1;
    let col = new Number( gid.substr(colIndex, 1) );
    g.board.setNumberAt(row, col, value);
    if( checkInputs() ) {
        let resultLabel = document.getElementById("result-label");
        let timeLabel = document.getElementById("time-label");
        let boardValid = g.board.isValid(true);
        if(boardValid) {
            resultLabel.innerHTML = `Game Done, You are RIGHT, time used: ${timeLabel.innerHTML}`;
            resultLabel.setAttribute("class", "result-right");
            tu.stop();
        }
        else {
            resultLabel.innerHTML = "Game Done, Some position may be WRONG";
            resultLabel.setAttribute("class", "result-wrong");
        }
    }
}

function checkInputs() {
    let valid = true;
    inputs.forEach( e => {
        if( !e.value ) {
            valid = false;
        }
    });
    return valid;
}

function changeDifficuty(value) {
    gameDifficuty = value;
}


genBoard();