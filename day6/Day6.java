import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

public class Day6 {
    private static int marker(String data, int markerLen)
    {
        for(int i = 0; i < data.length(); i++) {
            char[] buf = new char[markerLen];
            data.getChars(i, i + markerLen, buf, 0);
            Set<Character> s = new HashSet<Character>();
            for(char c : buf)
                s.add(c);
            if(s.size() == markerLen) {
                return i + markerLen;
            }
        }
        return 0;
    }

    public static void main(String[] args)
    {
        try {
            String data = Files.readString(Paths.get("input.txt"), Charset.defaultCharset());
            System.out.println(marker(data, 4));
            System.out.println(marker(data, 14));
        } catch(Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
