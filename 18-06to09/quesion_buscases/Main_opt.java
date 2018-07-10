import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * problem: 一条线路上有 n 个公交车站，假设在到达第 i 个站点之前，车上总共有 x 个乘客，过了该站点后车上总共有 y 个乘客，
 * 司机会把 y-x 这个数字，即乘客数量变化值 d_i 记录下来，公交车有固定的载客量 g，若有一份司机的开车日志，车上乘客数量的可能情况有多少种
 *
 * 输入：第一行有两个数字 n 和 g，分别表示站点数量及当前撤了的最大载客数量（1 <= n,g <= 1000）
 *       第二行共 n 个数字，由空格分开，经过第 i 个公交车站后车上乘客的变化数量 d_i (-1000 <= d_i <= 1000)
 *       输入数据保证从总站出发时乘客数量大于等于 0
 * 输出：输出一个数字，即司机从总站出发时，车上乘客数量的可能性个数。
 *
 * 样例输入一：
 * 4 10
 * 1 2 3 4
 * 样例输出一：
 * 1
 *
 * 样例输入二：
 * 4 5
 * 2 -1 2 1
 * 样例输出二：
 * 2
 */
class Main{
    private int station = 0;  // station 个公交车站
    private int carSize = 0;  // 固定载客量
    private int cases   = 0;  // 可能的情况数
    private int[] diff;        // 乘客数量变化值
    private int[] person;

    public static void main(String [] args){
        Main m = new Main();
        try {
            m.init();
            m.printPersonRange();
            m.findMinCase();
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println(m.cases);
    }

    private void init() throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        /*========= read first line to get variable n and g =========*/
        String firstLine  = br.readLine();
        String[] first = firstLine.split(" ");
        station = Integer.valueOf(first[0]);
        carSize = Integer.valueOf(first[1]);
        cases   = carSize+1;  // the maximum cases is carSize + 1 ( from 0 to carSize)
        /*===========================================================*/

        /*========= read second line to get variable d_i and initialize person =========*/
        diff = new int[station];
        person  = new int[(station+1)*2];  // odd means lower, even means upper
        String secondLine = br.readLine();
        String[] second = secondLine.split(" ");
        for (int i = 0; i < station; i++) {
            diff[i] = Integer.valueOf(second[i]);
            person[i*2] = 0;
            person[i*2+1] = carSize;
        }
        person[station*2] = 0;
        person[station*2+1] = carSize;
        /*========================================================*/

        int tmpMax = carSize, tmpMin = 0;

        // 算出 P0 的最小最大值
        person[0*2] = getMax(0, -diff[0]);
        person[0*2+1] = getMin(carSize, carSize - diff[0]);

        // 迭代算出 P1 - Pn-1 的最小最大值
        for(int i = 1; i < station; i++) {
            tmpMin = person[(i-1)*2];
            tmpMax = person[(i-1)*2+1];
            person[i*2]   = getMax( 0, tmpMin + diff[i-1], -diff[i] );
            person[i*2+1] = getMin( carSize, tmpMax + diff[i-1], carSize - diff[i] );
        }

        // 算出 Pn 的最小最大值
        tmpMin = person[(station-1)*2];
        tmpMax = person[(station-1)*2+1];
        person[station*2]   = tmpMin + diff[station-1];
        person[station*2+1] = tmpMax + diff[station-1];

    }

    /**
     * 求出所有站点可能情况的交集
     */
    private void findMinCase() {
        int tmpCase;
        for(int i = 0; i <= station; i++) {
            tmpCase = person[i*2+1]-person[i*2]+1;
            if( tmpCase < 0 ) {
                cases = 0;
                return;
            }
            if(cases > tmpCase) {
                cases = tmpCase;
            }
        }
    }

    /**
     * 打印出每个站点的可能的人数范围
     * @apiNote 第一个站点为总站
     */
    private void printPersonRange() {
        for (int i = 0; i < station+1; i++) {
            System.out.print( person[i*2] + "-" + person[i*2+1]+"  ");
        }
        System.out.println();
    }

    /**
     * 计算从 from (include) 到 to (not include) 站点的人数变化
     * @apiNote  第一站的序号为 0
     * 例如：
     * 经过四个站点的人数变化为： d_i = { 1, 2, 3, 4, -2, 3 }
     * personChanged(1, 5) 计算的值为 2 + 3 + 4 + (-2)，返回值为 7
     */
    private int personChanged(int from, int to) {
        int p = 0;
        for(int i = from; i < to; i++)
            p += diff[i];

        return p;
    }

    /**
     * 取 a，b中的最大值
     * @param a
     * @param b
     * @return
     */
    private int getMax(int a, int b) {
        return ( a > b ) ? a : b;
    }

    /**
     * 取 a，b，c 中的最大值
     * @param a
     * @param b
     * @param c
     * @return
     */
    private int getMax(int a, int b, int c) {
        int m;
        m = ( a > b ) ? a : b;
        m = ( m > c ) ? m : c;
        return m;
    }

    /**
     * 取 a，b中的最小值
     * @param a
     * @param b
     * @return
     */
    private int getMin(int a, int b) {
        return ( a < b ) ? a : b;
    }

    /**
     * 取 a，b，c 中的最小值
     * @param a
     * @param b
     * @param c
     * @return
     */
    private int getMin(int a, int b, int c) {
        int m;
        m = ( a < b ) ? a : b;
        m = ( m < c ) ? m : c;
        return m;
    }

}