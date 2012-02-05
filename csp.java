import java.io.*;
import java.util.Scanner;
/* In our CSP formulation, the marks are variables and the units in the ruler are domains */
/* marks are variables here and the terms are used interchangeably throughout the program */
/* A golumb ruler of order m is that it contains m marks */

class Csp
{
    /* length of the ruler we are testing */
    int length;
    /* number of marks in the ruler */
    int marks;
    /* number of constraing check */
    int cCheck;

    /* array representing ruler */
    boolean ruler[] = new boolean[1000];

    /* flag which keeps track of distance between all the existing marks */
    boolean flag[] = new boolean[1000];

    public void setLength(int i)
    {
        length = i;
    }
    public void setMarks(int i)
    {
        marks = i;
    }
    public void resetCCheck()
    {
        cCheck = 0;
    }
    public int getCCheck()
    {
        return cCheck;
    }
    public void clearRuler()
    {
        for(int i = 0; i < 1000; ++i)
            ruler[i] = false;
    }
    public void setRuler(int i)
    {
        ruler[i] = true;
    }
}

/*
   The idea behind MRV
   We have m marks {m1, m2, m3, m4.....mm}

   At the minimum, every mark will have its own boundary in which it should be placed. For e.g
   m2  > m1, m3 > m1 and m2 and so on.

   So,  m1 < m2 < m3 , m2 < m3 < m4 and so on.
   So, every time we choose the mark with less boundary in which it can be placed.

   How do we draw boundaries.
   claim: mark mi cannot be placed before i ( i + 1) / 2 positions from the start.

   proof:

   We need to have unique differences between every pair of marks.
   So, at the minimum if we start leaving spaces in the order 1, 2, 3, 4 between m0, m1, m2, m3 respectively,
   we can see the starting boundary of mark mi is i*(i+1)/2.

   We can't place mi before this because it would form a difference with an element which is already present in the sequence.

   There will be times at which mi exactly fits at the position i*(i+1)/2 and in most cases we need to find a position after this to fit mi.

   Same holds for right boundary of each marks. i.e  a mark m can be atmost placed at a distance i*(i+1)/2 from last if there are i elements after m.

 */

class CspMrv extends Csp
{
    /* contains the assigned variables. i.e fixed marks */
    int variables[] = new int[31];

    /* contains the left and right boundary for each variable */
    int mrvValue[][] = new int[31][2];

    /* since we are checking whether a ruler of particular length exists, fix the last mark */
    public void setLastVariable(int i)
    {
        variables[this.marks -1] = i;
    }

    /* flagging the differences made by placing a mark at ith position in ruler */
    /* this is an O(length) algorithm for checking all pair consistency. It greatly speeds up the backtracing.
       because O(m*m) > O(length). where m is number of marks and length is the length of ruler.
    */

    public void fixFlag(int i)
    {
        for(int k = 0; k <= length; ++k)
        {
            if(i == k)continue;
            if(ruler[k] == true)
                flag[Math.abs(i-k)] = true;
        }
    }

    /* unflagging the differences created by a mark placed at ith position, when removing that mark */
    /* Again O(length) algorithm */
    public void unfixFlag(int i)
    {
        for(int k = 0; k <= length; ++k)
        {
            if(i == k) continue;
            if(ruler[k] == true)
                flag[Math.abs(i-k)] = false;
        }
    }

    /* If all the marks are fixed properly, we are done */
    public boolean allFilled()
    {
        for(int i =0; i < marks; ++i)
        {
            if(variables[i] == -1)
                return false;
        }
        return true;
    }

    /* clear the array which contains the variable that are assigned i.e marks that are fixed */
    public void clearVariables()
    {
        for(int i  = 0; i < 31; ++i)
            variables[i] = -1;
    }

    /* initially set the boundary values like I mentioned earlier in my claim */
    public void setMrvs()
    {
        for(int i = 0; i < marks; ++i)
        {
            int end = marks - i - 1;
            mrvValue[i][0] = (i*(i+1))/2;
            mrvValue[i][1] = this.length - (end*(end+1))/2;
            if(i == 0)
                mrvValue[i][1] = 0;
        }
        System.out.println();
    }

    public CspMrv()
    {
        for(int i =0 ;i < marks; ++i)
        {
            mrvValue[i][1] = this.length;
            variables[i] = -1;
        }
    }

    /* update the boundary values for the unassigned variables based on the last placed mark */

    public void setMrv(int last)
    {
        int cnt = 0;
        int i = last;
        if(variables[i] != -1)
        {
            if(i+1 < length && variables[i+1] == -1)
                mrvValue[i+1][0] = variables[i] + 1;

            for(int j = i+2; j < marks; ++j)
            {
                if(variables[j] == -1)
                    mrvValue[j][0] = mrvValue[j-1][0] + 1;
            }
        }
    }

    /* returns the the unassigned variable which contains the least (its rightboundary - its leftboundary) value. i.e least space*/
    public int getMrv()
    {
        int res = 9999;
        int pos = 0;
        for(int i = 0; i < marks; ++i)
        {
            if(variables[i] != -1)
                continue;
            if((mrvValue[i][1] - mrvValue[i][0] + 1) <  res)
            {
                res = mrvValue[i][1] - mrvValue[i][0] + 1;
                pos = i;
            }
        }
        return pos;
    }

    /* check whether we can place the mark at position i in ruler. by comparing the difference it creates with other variables against already
       existing differences */

    boolean canFix(int i, boolean fc)
    {
        /* increment the constraint check */
        if(!fc)
            cCheck++;
        if(ruler[i] == true)
            return false;
        boolean mm[] = new boolean[500];
        for(int k = 0; k <= this.length; ++k)
        {
            if(k == i)
                continue;
            if(ruler[k] == true && (flag[Math.abs(k-i)] == true || mm[Math.abs(k-i)] == true))
                return false;
            else if(ruler[k] == true)
                mm[Math.abs(k-i)] = true;
        }
        return true;
    }

    /* Forward checking, when variable var is assigned to domain dom */
    public boolean fcOkay(int var, int dom)
    {
        int temp = variables[var];
        variables[var] = dom;
        ruler[dom] = true;
        fixFlag(dom);
        /* for all unassigned marks, check if the existing constraint is consistent */
        for(int i = 0 ; i < this.marks; ++i)
        {
            boolean flag = false;
            if(variables[i] == -1)
            {
                for(int j = 0; j<= this.length; ++j)
                {
                    if(canFix(j,true))
                    {
                        flag = true;
                        break;
                    }
                }
                /* inconsistent */
                if(flag == false)
                {
                    ruler[dom] = false;
                    unfixFlag(dom);
                    variables[var] = temp;
                    return false;
                }
            }
        }
        ruler[dom] = false;
        unfixFlag(dom);
        variables[var] = temp;
        return true;
    }

    /* check node consistency for constraint propagation.
       i.e  check whether the existing marks and their domains
       form a consistent relationship with the var if var gets assigned to dom.
       Do it for all unassigned variables. Keep on removing the inconsistent
       domain form variables.
    */

    public boolean cpOkay(int var, int dom)
    {
        int temp = variables[var];
        variables[var] = dom;
        ruler[dom] = true;
        fixFlag(dom);
        int cnt = 0;
        int varcnt = 0;
        for(int i = 0; i<= this.length; ++i)
        {
            if(canFix(i,true))
            {
                cnt++;
            }
        }
        for(int i = 0; i < marks;  ++i)
            if(variables[i] == -1)varcnt++;
        boolean ret;
        if(cnt >= varcnt)
            ret = true;
        else ret = false;
        ruler[dom] = false;
        unfixFlag(dom);
        variables[var] = temp;
        return ret;
    }

/* Backtracking with Mrv and it does Forward checking if fcswitch is set and Constraint propagation if cpswitch is set */

    public boolean backtrackMrv(int marks, int last, boolean fcswitch, boolean cpswitch)
    {
        /* If all marks are fixed. we are done */
        if(marks == 0 && allFilled())
            return true;

        if(allFilled() || marks == 0)
            return false;

        /* update the boundary values for all variables using the last set mark */
        setMrv(last);

        /* get the new MRV mark */
        int mrv = getMrv();

        /* iterate on all possible values(domains) in its boundary */
        for(int i = mrvValue[mrv][0]; i <= mrvValue[mrv][1]; ++i)
        {
            boolean res = false;
            int temp;
            if(canFix(i,false))
            {
                /* do forward checking if fcswitch is on */
                if(fcswitch)
                    if(!fcOkay(mrv,i))
                        continue;

                /* do constraint propagation if cpswitch is on */
                if(cpswitch)
                    if(!cpOkay(mrv,i))
                        continue;

                /* fix this mark at position i in ruler and recurse */
                temp = variables[mrv];
                ruler[i] = true;
                fixFlag(i);
                variables[mrv] = i;
                res = backtrackMrv(marks - 1, mrv, fcswitch, cpswitch);
                unfixFlag(i);
                variables[mrv] = temp;
                ruler[i] = false;
                if(res == true)
                    return res;
            }
        }
        return false;
    }
}

/* class for plain backtracking */

class CspBt extends Csp
{
    /* same as that of canFix() above */

    public boolean canPut(int i)
    {
        cCheck++;
        if(ruler[i] == true)
            return false;
        boolean mm[] = new boolean[500];
        for(int k = 0; k <= this.length; ++k)
        {
            if(k == i)
                continue;
            if(ruler[k] == true && (flag[Math.abs(k-i)] == true || mm[Math.abs(k-i)] == true))
                return false;
            else if(ruler[k] == true)
                mm[Math.abs(k-i)] = true;
        }
        return true;
    }

    /* flag the differences formed so far. Same as FixFlag() */
    public void setFlag(int i)
    {
        for(int k = 0; k <= this.length; ++k)
        {
            if(ruler[k] == true)
                flag[Math.abs(i-k)] = true;
        }
    }

    /* same as unFixFlag() */
    public void unsetFlag(int i)
    {
        for(int k = 0; k <= this.length; ++k)
        {
            if(ruler[k] == true)
                flag[Math.abs(i-k)] = false;
        }
    }

    /* just plain backtracing on marks */

    public boolean plainBacktrack(int marks)
    {
        if(marks == 0)
            return true;
        for(int i = 0; i <= length; ++i)
        {
            boolean res = false;
            if(canPut(i))
            {
                ruler[i] = true;
                setFlag(i);
                res = plainBacktrack(marks - 1);
                unsetFlag(i);
                ruler[i] = false;
            }
            if(res == true)
                return res;
        }
        return false;
    }
}


public class csp
{
    public static void main(String args[])
    {
        while(true)
        {
            Scanner scanner = new Scanner(System.in);
            System.out.print("Enter order and length of Golumb ruler (Enter 0 0 to exit): ");
            int i = scanner.nextInt();
            int j = scanner.nextInt();

            if(i == 0 && j == 0)
                break;

            /* csp with plain backtracking */
            boolean ans =false;
            if(i <= 10)
            {

                CspBt csbt = new CspBt();
                csbt.setLength(j);
                csbt.setMarks(i);
                csbt.setRuler(j);
                ans = csbt.plainBacktrack(i-1);
                if(ans)
                {
                    System.out.println(i + " " + j+" " + ans + ". plain backtracking consistency checks " + csbt.getCCheck());
                }
                else
                {
                    System.out.println("Fail for plain backtracking");
                }
            }

            /* csp with backtracking with mrv */


            CspMrv csbtmrv = new CspMrv();
            csbtmrv.setLength(j);
            csbtmrv.setMarks(i);
            csbtmrv.clearVariables();
            csbtmrv.setMrvs();
            csbtmrv.setRuler(j);
            csbtmrv.setLastVariable(j);
            ans = csbtmrv.backtrackMrv(i-1, 0, false, false);
            if(ans)
            {
                System.out.println(i + " " + j+ " " + ans + ". bt + mrv consistency checks "+ csbtmrv.getCCheck());
            }
            else
                    System.out.println("Fail for backtracking + mrv");

            /* csp with backtrackin , mrv and forward checking */

            CspMrv csbtmrvfc = new CspMrv();
            csbtmrvfc.setLength(j);
            csbtmrvfc.setMarks(i);
            csbtmrvfc.clearVariables();
            csbtmrvfc.setMrvs();
            csbtmrvfc.setRuler(j);
            csbtmrvfc.setLastVariable(j);
            ans = csbtmrvfc.backtrackMrv(i-1, 0, true, false);
            if(ans)
            {
                System.out.println(i + " " + j+ " " + ans + ". bt+ mrv + fc consistency checks "+ csbtmrvfc.getCCheck());
            }
            else
                System.out.println("Fail for backtracking + mrv + fc");

            /* csp with backtracing , mrv and constraint propagation */

            CspMrv csbtmrvcp = new CspMrv();
            csbtmrvcp.setLength(j);
            csbtmrvcp.setMarks(i);
            csbtmrvcp.clearVariables();
            csbtmrvcp.setMrvs();
            csbtmrvcp.setRuler(j);
            csbtmrvcp.setLastVariable(j);
            ans = csbtmrvcp.backtrackMrv(i-1, 0, false, true);
            if(ans)
            {
                System.out.println(i + " " + j+ " " + ans + ". bt + mrv + cp consistency checks "+ csbtmrvcp.getCCheck());
            }
            else
                System.out.println("Fail for backtracking + mrv + cp");

        }



// The below commented code generates all optimal golumb rulers of order upto 12 and length 85. False cases take more time since it exhausts the recursion tree.


/*
        // trying to generate all Golumbs ruler with marks <= 9 and length <= 100
        // plain backtracking

        for(int i = 1; i <= 8; ++i)
        {
            for(int j = 0; j <= 100; ++j)
            {
                CspBt csbt = new CspBt();
                csbt.setLength(j);
                csbt.setMarks(i);
                csbt.setRuler(j);
                boolean ans = csbt.plainBacktrack(i-1);
                if(ans)
                {
                    System.out.println(i + " " + j+" " + ans + ". Consistency checks " + csbt.getCCheck());
                    break;
                }
            }
        }

        System.out.println("*****************");

        // trying to generate all Golumbs ruler with marks <= 12 and length <= 100
        // back tracking with mrv
        for(int i = 1; i <= 10; ++i)
        {
            for(int j = 0 ; j <= 100; ++j)
            {
                CspMrv csbtmrv = new CspMrv();
                csbtmrv.setLength(j);
                csbtmrv.setMarks(i);
                csbtmrv.clearVariables();
                csbtmrv.setMrvs();
                csbtmrv.setRuler(j);
                csbtmrv.setLastVariable(j);
                boolean ans = csbtmrv.backtrackMrv(i-1, 0, false, false);
                if(ans)
                {
                    System.out.println(i + " " + j+ " " + ans + ". Consistency checks "+ csbtmrv.getCCheck());
                    break;
                }
            }
        }
        System.out.println("******************");


        // trying to generate all Golumbs ruler with marks <= 12 and length <= 100
        // backtracking with mrv and forward checking

        for(int i = 1; i <= 10; ++i)
        {
            for(int j = 0 ; j <= 100; ++j)
            {
                CspMrv csbtmrvfc = new CspMrv();
                csbtmrvfc.setLength(j);
                csbtmrvfc.setMarks(i);
                csbtmrvfc.clearVariables();
                csbtmrvfc.setMrvs();
                csbtmrvfc.setRuler(j);
                csbtmrvfc.setLastVariable(j);
                boolean ans = csbtmrvfc.backtrackMrv(i-1, 0, true, false);
                if(ans)
                {
                    System.out.println(i + " " + j+ " " + ans + ". Consistency checks "+ csbtmrvfc.getCCheck());
                    break;
                }
            }
        }
        System.out.println("******************");


        // trying to generate all Golumbs ruler with marks <= 12 and length <= 100
        // backtracking with mrv, forward checking and constraint propagation

        for(int i = 1; i <= 12; ++i)
        {
            for(int j = 0 ; j <= 100; ++j)
            {
                CspMrv csbtmrvfccp = new CspMrv();
                csbtmrvfccp.setLength(j);
                csbtmrvfccp.setMarks(i);
                csbtmrvfccp.clearVariables();
                csbtmrvfccp.setMrvs();
                csbtmrvfccp.setRuler(j);
                csbtmrvfccp.setLastVariable(j);
                boolean ans = csbtmrvfccp.backtrackMrv(i-1, 0, true, true);
                if(ans)
                {
                    System.out.println(i + " " + j+ " " + ans + ". Consistency checks "+ csbtmrvfccp.getCCheck());
                    break;
                }
            }
        }
        System.out.println("******************");
     */
    }
}