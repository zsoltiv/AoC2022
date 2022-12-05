import java.io.BufferedReader;
import java.io.FileReader;

public class Day4 {
    private static boolean contains(int[] a, int[] b)
    {
        boolean res = (a[0] >= b[0] && a[1] <= b[1]) || (b[0] >= a[0] && b[1] <= a[1]);
        System.out.println(a[0]+"-"+a[1]+"\t"+b[0]+"-"+b[1]+"\t"+res);
        return res;
    }

    private static boolean overlaps(int[] a, int[] b)
    {
        return (a[0] >= b[0] && a[0] <= b[1]) || (b[0] >= a[0] && b[0] <= a[1]) ||
                (a[1] >= b[0] && a[1] <= b[1]) || (b[1] >= a[0] && b[1] <= a[1]);
    }

    public static void main(String[] args)
    {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line = reader.readLine();

            int fullyContains = 0;
            int overlapping = 0;
            while(line != null) {
                String[] rangeStrings = line.split(",");
                int[][] ranges = new int[2][2];
                for(int j = 0; j < ranges.length; j++) {
                    for(int k = 0; k < ranges[0].length; k++) {
                        ranges[j][k] = Integer.parseInt(rangeStrings[j].split("-")[k]);
                    }
                }
                fullyContains = contains(ranges[0], ranges[1]) ? fullyContains + 1 : fullyContains;
                overlapping = overlaps(ranges[0], ranges[1]) ? overlapping + 1 : overlapping;
                line = reader.readLine();
            }

            reader.close();

            System.out.println(fullyContains);
            System.out.println(overlapping);
        } catch(Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
