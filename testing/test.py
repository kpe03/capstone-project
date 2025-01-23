import unittest

class TestPriorityAlgorithm(unittest.TestCase):
    def calc_difference(self, final, desired, key):
        if desired[key] == 0:  # Prevent division by zero
            return float('inf')  
        return abs(final[key] - desired[key]) / desired[key] * 100

    def test_priorities_match(self):
        # input data

        final = {'temperature': 19, 'humidity': 31, 'energy': 100}
        # final = {'temperature': 19, 'humidity': 50, 'energy': 80}
        desired = {'temperature': 20, 'humidity': 35, 'energy': 70}
        priorities = ['temperature', 'humidity', 'energy']

        # Calculate percentage differences 
        differences = {}
        for key in final:
            differences[key] = self.calc_difference(final, desired, key)

        # Verify highest priority should have the lowest difference
        for i in range(len(priorities) - 1):
            key_high = priorities[i]  # Higher priority key
            key_low = priorities[i + 1]  # Lower priority key
            self.assertLessEqual(
                differences[key_high],
                differences[key_low],
                f"{key_high} should have a lower or equal difference than {key_low}."
            )

if __name__ == "__main__":
    unittest.main()
