# Buckeye Buttons Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Anyone taken Software 1? Can you help me figure out what this weird Java program does?

## Write up:

First thing I did when I noticed the .jar file was to decompile it back to java, when I did that I got the following:

```java
import java.awt.event.ActionEvent;
import javax.swing.JOptionPane;
import java.nio.charset.StandardCharsets;
import java.math.BigInteger;
import java.util.Iterator;
import java.awt.Dimension;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.LayoutManager;
import java.awt.Container;
import javax.swing.BoxLayout;
import javax.swing.JPanel;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.event.ActionListener;
import javax.swing.JFrame;

// 
// Decompiled by Procyon v0.5.36
// 

public final class Buttons extends JFrame implements ActionListener
{
    private static final int[][] grid;
    private final int rows;
    private final int cols;
    private final JLabel movesLabel;
    private final JButton resetButton;
    private final JButton[][] buttons;
    private int posRow;
    private int posCol;
    private final int endRow;
    private final int endCol;
    private static final int MAX_MOVES = 139;
    private ArrayList<Integer> moveHistory;
    
    public static void main(final String[] array) throws Exception {
        new Buttons();
    }
    
    public Buttons() {
        super("Buttons!");
        this.resetValues();
        this.rows = Buttons.grid.length;
        this.cols = Buttons.grid[0].length;
        this.endRow = this.rows - 1;
        this.endCol = this.cols - 2;
        final JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, 3));
        final JPanel comp = new JPanel(new GridLayout(this.rows, this.cols));
        final JPanel comp2 = new JPanel();
        this.buttons = new JButton[this.rows][this.cols];
        for (int i = 0; i < this.rows; ++i) {
            for (int j = 0; j < this.cols; ++j) {
                (this.buttons[i][j] = new JButton("?")).addActionListener(this);
                this.buttons[i][j].setActionCommand(Integer.toString(j + i * this.cols));
                comp.add(this.buttons[i][j]);
            }
        }
        this.buttons[this.endRow][this.endCol].setText("\u2691");
        comp.setPreferredSize(new Dimension(45 * this.rows, 45 * this.cols));
        comp2.add(this.movesLabel = new JLabel("Moves left: 20"));
        (this.resetButton = new JButton("Reset")).addActionListener(this);
        this.resetButton.setActionCommand("reset");
        comp2.add(this.resetButton);
        panel.add(comp);
        panel.add(comp2);
        this.resetGUI();
        this.getContentPane().add(panel);
        this.pack();
        this.setDefaultCloseOperation(3);
        this.setVisible(true);
    }
    
    private void resetValues() {
        this.posRow = 0;
        this.posCol = 1;
        (this.moveHistory = new ArrayList<Integer>()).add(this.posCol + this.posRow * this.cols);
    }
    
    private void updateMovesLeft() {
        this.movesLabel.setText(invokedynamic(makeConcatWithConstants:(Ljava/lang/String;)Ljava/lang/String;, Integer.toString(139 - this.moveHistory.size())));
    }
    
    private void resetGUI() {
        for (int i = 0; i < this.rows; ++i) {
            for (int j = 0; j < this.cols; ++j) {
                this.buttons[i][j].setEnabled(true);
            }
        }
        for (final int intValue : this.moveHistory) {
            this.buttons[intValue / this.cols][intValue % this.cols].setEnabled(false);
        }
        this.updateMovesLeft();
    }
    
    private void reset() {
        this.resetValues();
        this.resetGUI();
    }
    
    private boolean isLegalMove(final int n, final int n2) {
        return 139 - this.moveHistory.size() > 0 && Buttons.grid[n][n2] == 0 && Math.abs(n - this.posRow) + Math.abs(n2 - this.posCol) == 1;
    }
    
    private void printFlag() {
        final BigInteger[] array = new BigInteger[this.moveHistory.size()];
        array[0] = BigInteger.valueOf(2L);
        for (int i = 1; i < array.length; ++i) {
            array[i] = array[i - 1].nextProbablePrime();
        }
        BigInteger val = BigInteger.valueOf(1L);
        final BigInteger m = new BigInteger("1430313837704837266267655033918654049072573502772041995300810633148485540425442305963378206448908414865491202671058946396326575688430628383447817933039379");
        for (int j = 0; j < this.moveHistory.size(); ++j) {
            val = val.multiply(array[j].modPow(BigInteger.valueOf(this.moveHistory.get(j)), m)).mod(m);
        }
        final String x = new String(new BigInteger("1181624346478884506978387685027501257422054115549381320819711748725513305918055802813085700551988448885328987653245675378090761255233757606571908411691314").multiply(val).mod(m).toByteArray(), StandardCharsets.UTF_8);
        JOptionPane.showMessageDialog((Component)this, invokedynamic(makeConcatWithConstants:(Ljava/lang/String;)Ljava/lang/String;, x), "Flag", 1);
        System.out.println(x);
    }
    
    @Override
    public void actionPerformed(final ActionEvent actionEvent) {
        final String actionCommand = actionEvent.getActionCommand();
        if (actionCommand.equals("reset")) {
            this.reset();
        }
        else {
            int int1 = Integer.parseInt(actionCommand);
		    final int posRow = int1 / this.cols;
            final int posCol = int1 % this.cols;
		    if (this.isLegalMove(posRow, posCol)) {
		        this.buttons[posRow][posCol].setEnabled(false);
		        this.posRow = posRow;
		        this.posCol = posCol;
		        this.moveHistory.add(int1);
		        System.out.println(this.moveHistory);
		        this.updateMovesLeft();
		        if (this.posRow == this.endRow && this.posCol == this.endCol) {
		            this.printFlag();
		        }
			break;
		    }
            
            }
            else {
                JOptionPane.showMessageDialog(this, "Illegal move, you lose \u2639", "Illegal move", 0);
                this.reset();
            }
        }
    }
    
    static {
        grid = new int[][] { { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, { 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 }, { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1 }, { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 }, { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 }, { 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1 }, { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 }, { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 } };
    }
}
```

The first thing I noticed was the print flag function:

```java
private void printFlag() {
        final BigInteger[] array = new BigInteger[this.moveHistory.size()];
        array[0] = BigInteger.valueOf(2L);
        for (int i = 1; i < array.length; ++i) {
            array[i] = array[i - 1].nextProbablePrime();
        }
        BigInteger val = BigInteger.valueOf(1L);
        final BigInteger m = new BigInteger("1430313837704837266267655033918654049072573502772041995300810633148485540425442305963378206448908414865491202671058946396326575688430628383447817933039379");
        for (int j = 0; j < this.moveHistory.size(); ++j) {
            val = val.multiply(array[j].modPow(BigInteger.valueOf(this.moveHistory.get(j)), m)).mod(m);
        }
        final String x = new String(new BigInteger("1181624346478884506978387685027501257422054115549381320819711748725513305918055802813085700551988448885328987653245675378090761255233757606571908411691314").multiply(val).mod(m).toByteArray(), StandardCharsets.UTF_8);
        JOptionPane.showMessageDialog((Component)this, invokedynamic(makeConcatWithConstants:(Ljava/lang/String;)Ljava/lang/String;, x), "Flag", 1);
        System.out.println(x);
    }
```

Sadly the flag depends on the move history so we can't just run this function to get the flag. So the next thing I did was look into the function that calls the print flag:

```java
    @Override
    public void actionPerformed(final ActionEvent actionEvent) {
        final String actionCommand = actionEvent.getActionCommand();
        if (actionCommand.equals("reset")) {
            this.reset();
        }
        else {
            int int1 = Integer.parseInt(actionCommand);
		    final int posRow = int1 / this.cols;
            final int posCol = int1 % this.cols;
		    if (this.isLegalMove(posRow, posCol)) {
		        this.buttons[posRow][posCol].setEnabled(false);
		        this.posRow = posRow;
		        this.posCol = posCol;
		        this.moveHistory.add(int1);
		        System.out.println(this.moveHistory);
		        this.updateMovesLeft();
		        if (this.posRow == this.endRow && this.posCol == this.endCol) {
		            this.printFlag();
		        }
			break;
		    }
            
            }
            else {
                JOptionPane.showMessageDialog(this, "Illegal move, you lose \u2639", "Illegal move", 0);
                this.reset();
            }
        }
    }
```

We check if the currently selected item is a legal move, if it is then we add it to the history, check if we are at the end, and if we are then we print out the flag.

At this point I had written a script and noticed that there were several different paths with only one of them getting you to the end so I decided to make a script using a breadth first search algorithm:

```py
# import math 
import math

# initialize start values
grid = [ [ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ], [ 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 ], [ 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 ], [ 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 ], [ 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 ], [ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 ], [ 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 ], [ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 ], [ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 ], [ 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 ], [ 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1 ], [ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 ], [ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 ], [ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 ], [ 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1 ], [ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 ], [ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ], [ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 ], [ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 ], [ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 ], [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 ] ];
gridLen = 441
MAX_MOVES = 139
rows = len(grid)
cols = len(grid[0])
endRow = rows - 1
endCol = cols - 2

# all paths that are possible
paths = []

# Class for a path
class Path:
	def __init__(self, posRow, posCol, history):
		self.posRow = posRow
		self.posCol = posCol
		self.history = history

# check if a move is legal
def isLegal(path, posRow, posCol):
	return (139 - len(path.history) > 0) and (grid[posRow][posCol] == 0) and (math.fabs(posRow - path.posRow) + math.fabs(posCol - path.posCol) == 1)

# append the first path to the array
paths.append(Path(0, 1, [1]))

# while there are paths 
while len(paths) > 0:

    # get the first path
	path = paths[0]
	
    # loop through all of the grid indices
	for i in range(0, 441):

        # if i is not in the paths history (we haven't already gone to this path, stops looping back)
		if i not in path.history:

            # gets the rows and columns
			posRow = i // cols
			posCol = i % cols

            # check if the path is legal
			if isLegal(path, posRow, posCol):

                # copy the history from the original path and append the current location
				history = path.history.copy()
				history.append(i)

                # create a new path for this move we just found
				paths.append(Path(posRow, posCol, history))

                # if it is the end then print out the moves
				if posRow == endRow and posCol == endCol:
					for x in history:

                        # print in a nice format to paste into our java file
						print("moveHistory.add(" + str(x) + ");")
					print("Finished ", history)

    # remove the first path from the array
	paths.pop(0)
```

When run this output a bunch of java add to arraylist code which I then pasted into a java file I had made so I could just run it with the print flag code:

```java
import java.awt.event.ActionEvent;
import javax.swing.JOptionPane;
import java.nio.charset.StandardCharsets;
import java.math.BigInteger;
import java.util.Iterator;
import java.awt.Dimension;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.LayoutManager;
import java.awt.Container;
import javax.swing.BoxLayout;
import javax.swing.JPanel;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.event.ActionListener;
import javax.swing.JFrame;

public class run
{
	private static final int[][] grid = new int[][] { { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, { 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 }, { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1 }, { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 }, { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 }, { 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1 }, { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 }, { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 } };

    private static int rows = grid.length;
    private static int cols = grid[0].length;
    private static int gPosRow = 0;
    private static int gPosCol = 1;
    
    private static final int endRow = rows - 1;
    private static final int endCol = cols - 2;
    private static final int MAX_MOVES = 139;
    private static ArrayList<Integer> moveHistory = new ArrayList<Integer>();

private static boolean isLegalMove(final int n, final int n2) {
        return 139 - moveHistory.size() > 0 && grid[n][n2] == 0 && Math.abs(n - gPosRow) + Math.abs(n2 - gPosCol) == 1;
    }

    private static void printFlag() {
        final BigInteger[] array = new BigInteger[moveHistory.size()];
        array[0] = BigInteger.valueOf(2L);
        for (int i = 1; i < array.length; ++i) {
            array[i] = array[i - 1].nextProbablePrime();
        }
        BigInteger val = BigInteger.valueOf(1L);
        final BigInteger m = new BigInteger("1430313837704837266267655033918654049072573502772041995300810633148485540425442305963378206448908414865491202671058946396326575688430628383447817933039379");
        for (int j = 0; j < moveHistory.size(); ++j) {
            val = val.multiply(array[j].modPow(BigInteger.valueOf(moveHistory.get(j)), m)).mod(m);
        }
        final String x = new String(new BigInteger("1181624346478884506978387685027501257422054115549381320819711748725513305918055802813085700551988448885328987653245675378090761255233757606571908411691314").multiply(val).mod(m).toByteArray(), StandardCharsets.UTF_8);
        System.out.println(x);
    }

	public static void main(final String[] array) throws Exception {
		moveHistory.add(1);
        moveHistory.add(22);
        moveHistory.add(43);
        moveHistory.add(64);
        moveHistory.add(85);
        moveHistory.add(106);
        moveHistory.add(127);
        moveHistory.add(148);
        moveHistory.add(149);
        moveHistory.add(150);
        moveHistory.add(151);
        moveHistory.add(152);
        moveHistory.add(131);
        moveHistory.add(110);
        moveHistory.add(89);
        moveHistory.add(68);
        moveHistory.add(67);
        moveHistory.add(66);
        moveHistory.add(45);
        moveHistory.add(24);
        moveHistory.add(25);
        moveHistory.add(26);
        moveHistory.add(27);
        moveHistory.add(28);
        moveHistory.add(49);
        moveHistory.add(70);
        moveHistory.add(71);
        moveHistory.add(72);
        moveHistory.add(51);
        moveHistory.add(30);
        moveHistory.add(31);
        moveHistory.add(32);
        moveHistory.add(33);
        moveHistory.add(34);
        moveHistory.add(55);
        moveHistory.add(76);
        moveHistory.add(77);
        moveHistory.add(78);
        moveHistory.add(79);
        moveHistory.add(80);
        moveHistory.add(81);
        moveHistory.add(82);
        moveHistory.add(103);
        moveHistory.add(124);
        moveHistory.add(123);
        moveHistory.add(122);
        moveHistory.add(143);
        moveHistory.add(164);
        moveHistory.add(163);
        moveHistory.add(162);
        moveHistory.add(161);
        moveHistory.add(160);
        moveHistory.add(139);
        moveHistory.add(118);
        moveHistory.add(117);
        moveHistory.add(116);
        moveHistory.add(115);
        moveHistory.add(114);
        moveHistory.add(135);
        moveHistory.add(156);
        moveHistory.add(155);
        moveHistory.add(154);
        moveHistory.add(175);
        moveHistory.add(196);
        moveHistory.add(195);
        moveHistory.add(194);
        moveHistory.add(193);
        moveHistory.add(192);
        moveHistory.add(213);
        moveHistory.add(234);
        moveHistory.add(233);
        moveHistory.add(232);
        moveHistory.add(253);
        moveHistory.add(274);
        moveHistory.add(275);
        moveHistory.add(276);
        moveHistory.add(277);
        moveHistory.add(278);
        moveHistory.add(299);
        moveHistory.add(320);
        moveHistory.add(321);
        moveHistory.add(322);
        moveHistory.add(301);
        moveHistory.add(280);
        moveHistory.add(281);
        moveHistory.add(282);
        moveHistory.add(261);
        moveHistory.add(240);
        moveHistory.add(219);
        moveHistory.add(198);
        moveHistory.add(199);
        moveHistory.add(200);
        moveHistory.add(221);
        moveHistory.add(242);
        moveHistory.add(243);
        moveHistory.add(244);
        moveHistory.add(245);
        moveHistory.add(246);
        moveHistory.add(247);
        moveHistory.add(248);
        moveHistory.add(227);
        moveHistory.add(206);
        moveHistory.add(207);
        moveHistory.add(208);
        moveHistory.add(229);
        moveHistory.add(250);
        moveHistory.add(271);
        moveHistory.add(292);
        moveHistory.add(313);
        moveHistory.add(334);
        moveHistory.add(333);
        moveHistory.add(332);
        moveHistory.add(331);
        moveHistory.add(330);
        moveHistory.add(309);
        moveHistory.add(288);
        moveHistory.add(287);
        moveHistory.add(286);
        moveHistory.add(307);
        moveHistory.add(328);
        moveHistory.add(349);
        moveHistory.add(370);
        moveHistory.add(369);
        moveHistory.add(368);
        moveHistory.add(389);
        moveHistory.add(410);
        moveHistory.add(411);
        moveHistory.add(412);
        moveHistory.add(413);
        moveHistory.add(414);
        moveHistory.add(393);
        moveHistory.add(372);
        moveHistory.add(373);
        moveHistory.add(374);
        moveHistory.add(375);
        moveHistory.add(376);
        moveHistory.add(397);
        moveHistory.add(418);
        moveHistory.add(439);
        printFlag();
    	}	
}
```

When run this output:

```
buckeye{am4z1ng_j0b_y0u_b1g_j4va_h4ck3r}
```