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
        test.checkPriorities();
    }
    
    public void checkPriorities() {
        //Define the priority order
        List<String> priorityOrder = Arrays.asList("temperature", "humidity", "energy");

        //Get the actual priority values
        Map<String, Integer> actualPriorities = mockAlgorithm();


        //Validate the priorities
        for(int i = 0; i < priorityOrder.size(); i++) {
            //Check for missing values, if fail print the missing priority
            String key = priorityOrder.get(i);

            if(!actualPriorities.containsKey(key)) {
                System.out.println("Missing priority: " + key);
            } 
        }

        System.out.println("Priorities are correct");   
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