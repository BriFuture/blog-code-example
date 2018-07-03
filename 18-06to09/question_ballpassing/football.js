// Ａ/Ｂ/Ｃ/Ｄ/Ｅ五个人互相传球，由A开始第一次传球，经5次传球后传回到A的手上，其中A与B不会相互传球，C只会传给Ｄ，Ｅ不会传给Ｃ，共有多少种传法？

class Person {
    constructor(name) {
        this.name = name;
        this.ableToNext = new Array();
    }

    /**
     * add the person that this one can pass the ball to
     * @param {Person} person 
     */
    addNext(person) {
        this.ableToNext.push(person);
    }

    /**
     * can this person pass the ball to certain person
     * @param {Person} to 
     */
    isAbleToNext(to) {
        let canPass = false;
        this.ableToNext.forEach(p => {
            if( p == to ) {
                canPass = true;
            }
        });
        return canPass;
    }

    /**
     * to show this one's name
     */
    toString() {
        return this.name;
    }
}

const MaxBallPassCount = 5;
var limits = { 
    'A': ['C', 'D', 'E'],   // A can pass ball to C, D or E
    'B': ['C', 'D', 'E'],   // B can pass ball to C, D or E
    'C': ['D'],             // C can pass ball to D;
    'D': ['A', 'B', 'C', 'E'],  // D can pass ball to A, B, C or E
    'E': ['A', 'B', 'D']        // E can pass ball to A, B or E
}

var names = Object.keys(limits);
var persons = {};
var attmptCount = 0;
var validCount = 0;
var ballStack = new Array();

// initialization 
function init() {
    // construct all person
    names.forEach( name => {
        persons[name] = new Person( name );
    });
    // add limitations to certain person
    names.forEach( name => {
        limits[name].forEach( limit => {
            persons[name].addNext(persons[limit]);
        })
    });
    // the fisrt one who owns the ball should be the last one who owns the ball
    persons.ballOwner  = persons[ names[0] ];
    persons.finalOwner = persons[ names[0] ];
}

/**
 * @param {Person} person 
 */
function passBall(person) {
    // end condition of recursive
    if( ballStack.length == MaxBallPassCount ) {
        if( persons.ballOwner == persons.finalOwner ) {
            validCount++;
            console.log( validCount, ballStack );
        }
        ballStack.pop();
        return;
    }

    person.ableToNext.forEach( next => {
        // /*
        if( ballStack.length < (MaxBallPassCount - 1) && next == persons.finalOwner ) {
            // finalOwner doesn't get the ball at the meantime
            return;
        }
        // */
        attmptCount++;
        ballStack.push( next.name );
        persons.ballOwner = next;
        // log( ballStack.length +": "+ next+ "  " );
        passBall(next);
    });
    // all possiblility have been attempted, get back to last one
    ballStack.pop();
}

/* // test 
var log = function (d) {
    if( d.toString ) 
        process.stdout.write(d.toString());
    else
        process.stdout.write(d);
};

names.forEach( name => {
    console.log( persons[name] + " ==> " + persons[name].ableToNext)
})
*/

(function main(){
    init();
    // start passing the ball
    passBall( persons.ballOwner );
    console.log( "Attmpt: " + attmptCount + "  Valid: " + validCount );
})()

