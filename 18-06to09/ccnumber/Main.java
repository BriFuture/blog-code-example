import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * @author BriFuture
 * @date   2018.09.20
 * @file   Main.java
 *
 */

public class Main {
    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String NStr = br.readLine();
        int N = Integer.parseInt( NStr );

        int[] aN = new int[ N ];
        int[] bN = new int[ N ];
        // 读取各组数据
        for( int i = 0; i < N; i++ ) {
            String[] data = br.readLine().split(" ");
            aN[i] = Integer.parseInt( data[0] );  // 区间的左边界
            bN[i] = Integer.parseInt( data[1] );  // 区间的右边界
        }

        for( int i = 0; i < N; i++ ) {
            int score = find( aN[i], bN[i] );
            System.out.println( "Case " + (i+1) + ": " + score );
        }
    }

    public static int find( int A, int B ) {
        int score = 0;

        for( int n = A; n <= B; n++ ) {
            String nStr = String.valueOf( n );
            // 检查 n 字符串是不是 cc number，并且返回它的 score
            int tmpScore;
            // tmpScore = check( nStr );  
            tmpScore = checkWithFSM( nStr );
            if( tmpScore > score )
                score = tmpScore;
        }
        return score;  // 返回的是 A-B 区间内的 CC Number 的最大 Score
    }

    public static int check( String nStr ) {
        char cp, cb;  // pre, back
        int score = 0;
        int up1Start = -1, down1Start = -1, down1End = -1;
        int up2Start = -1, down2Start = -1;
        boolean isCCNumber = false;
        for( int j = 0; j < nStr.length() - 1; j++ ) {
            cp = nStr.charAt( j + 1 );
            cb = nStr.charAt( j );  // current char

            if( up1Start == -1 ) {
                if( cp > cb) {
                    // start up1
                    up1Start = j;
                }
            }
            else {
                if( down1Start == -1 ) {
                    // on up1
                    if( cp == cb ) {
                        up1Start = -1; // reset up1start
                    }
                    else if( cp < cb ) {
                        // start down1
                        down1Start = j;
                    }  // no need to process case cp > cb again

                }
                else {
                    if( down1End == -1 ) {
                        // on down1
                        if (cp >= cb) {
                            down1End = j; // finish down1
                        } // no need to process case cp < cb
                    }
                    else {
                        // check up-down second time
                        if( up2Start == -1 ) {
                            if( cp > cb && cb != '0' ) {
                                // start up2, cb can not be '0'
                                up2Start = j;
                                if( up2Start - down1End != 1 ) {
                                    up1Start = -1;
                                    down1Start = -1;
                                    down1End = -1;
                                    up2Start = -1;
                                    continue;
                                }
                            }
                        }
                        else {
                            if( down2Start == -1 ) {
                                // on up2
                                if( cp == cb ) {
                                    up2Start = -1; // reset up1start
                                }
                                else if( cp < cb ) {
                                    // start down2
                                    down2Start = j;
                                    isCCNumber = true;
                                    break; // no need to know where down2 ends
                                }  // no need to process case cp > cb again

                            }
                            // else {
                            //    if( down2End == -1 ) {
                            //        // on down2
                            //        if (cp >= cb) {
                            //            down2End = j;
                            //        } // no need to process case cp < cb
                            //    }
                            // }
                        }
                    }
                }
            }
        }
        if( isCCNumber )
            score = calcScore( nStr );

        return score;
    }

    public enum State { Start, Stage1Up,Stage1Down, StageNext, Stage2Up, /*Stage2Down*/};
    public static int checkWithFSM( String nStr ) {
        State currState = State.Start;
        char cp, cb;  // pre, back
        int j = 0;
        while( j < nStr.length() - 1 ) {
            cp = nStr.charAt( j + 1 );  // next char
            cb = nStr.charAt( j );      // current char
            switch ( currState ) {
                case Start:
                    if( cp > cb )
                        currState = State.Stage1Up;  // state State1Up
                    else
                        currState = State.Start;
                    break;
                case Stage1Up:
                    if( cp == cb )
                        currState = State.Start;  // reset
                    else if( cp < cb )
                        currState = State.Stage1Down; // end of Stage1Up
                    // else Stage1Up
                    break;
                case Stage1Down:
                    if( cp >= cb ) {
                        currState = State.StageNext;  // end of Stage1Down
                    }
                    // else Stage1Down
                    break;
                case StageNext:
                    if( cp > cb && cb != '0' )  // maximum number can not be '0'
                        currState = State.Stage2Up;  // start  State2Up
                    else
                        currState = State.Start;  // Stage1Down 和 Stage2Up 不相邻，reset
                    break;
                case Stage2Up:
                    if( cp == cb ) {
                        currState = State.Start; // not on up, reset currState
                    }
                    else if( cp < cb ) {
//                        currState = State.Stage2Down; // start down
                        return calcScore( nStr );
                    }
                    // else Stage2Up
                    break;
//                case Stage2Down:  // no need to care about Stage2Down state

            }
            j++;
        }

        return 0;
    }

    public static int calcScore( String nStr ) {
        char c;
        int score = 0;
        for( int i = 0; i < nStr.length(); i++ ) {
            c = nStr.charAt( i );
            score += ( c - '0' );
        }
        return score;
    }
}

/*
4
12121 12121
120010 120010
121121 121121
1211121 1211121
 */