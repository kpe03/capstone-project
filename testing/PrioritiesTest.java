/*  
    Description: Unit Test in Java
    Kaitlyn Peters 
    Ticket 2
    Date: 1/23/2025
*/

/* Verify the priorities in a list are correct */
import java.util.*;

public class PrioritiesTest {
    /*
    *   Verify that the list has the correct priorities
    */
    public static void main(String[] args) {
        PrioritiesTest test = new PrioritiesTest();
        test.testPriorityValue();
    }
    
    public void testPriorityValue() {
        //Define the priority order
        Map<String, Integer> desired = new HashMap<>();
        desired.put("temperature", 20);
        desired.put("humidity", 35);
        desired.put("energy", 70);

        //Get the actual priority values
        Map<String, Integer> actualPriorities = mockAlgorithm();

        List<String> priorities = Arrays.asList("temperature", "humidity", "energy");

        //get percentage differences
        Map<String, Double> differences = new HashMap<>();

        //Validate the priorities
        for(String key : priorities) {
            //Check for missing values, if fail print the missing priority

            //get the priority values
            int finalValue = actualPriorities.get(key);
            int desiredValue = desired.get(key);

            //calculate the percentage difference
            double percentageDifference = Math.abs(finalValue - desiredValue) / (double) desiredValue * 100;
            differences.put(key, percentageDifference);
        }

        //check percentage differences follow priority order
        for (int i = 0; i < differences.size() - 1; i++) {
            String keyHigh = priorities.get(i); // Higher priority key
            String keyLow = priorities.get(i + 1); // Lower priority key

            if (!(differences.get(keyHigh) <= differences.get(keyLow))) {
            System.out.printf("Priority order is incorrect: %s should have a lower or equal difference than %s%n", keyHigh, keyLow);
            return;
            }
            else {
                System.out.printf("Priority order is correct: %s has a lower or equal difference than %s%n", keyHigh, keyLow);
            }
        }
    }

    /*
    * An example algorithm that returns a HashMap with the properties and their values
    */
    private Map<String, Integer> mockAlgorithm() {
        Map<String, Integer> result = new HashMap<>();
        result.put("temperature", 21); // Example result close to desired
        result.put("humidity", 38);
        result.put("energy", 80);
        return result;
    }
}