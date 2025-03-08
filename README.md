# Parallel Development Plan (chat gpt :p)

| Task                                        | Developer    | Dependencies                                    |
| ------------------------------------------- | ------------ | ----------------------------------------------- |
| Configure I2C (Master/Slave)                | **Person A** | None                                            |
| Configure GPIOs (Buttons & LEDs)            | **Person B** | None                                            |
| Implement button interrupt handling         | **Person B** | Needs GPIO setup                                |
| Implement LED countdown sequence            | **Person B** | Needs GPIO setup                                |
| Implement I2C send & receive functions      | **Person A** | Needs I2C setup                                 |
| Implement game state transitions            | **Person A** | Needs I2C communication & button press handling |
| Implement reaction detection & winner logic | **Person A** | Needs game state management & button interrupts |
| Implement LED signals for winner/loser      | **Person B** | Needs reaction detection                        |
| Implement game reset logic                  | **Person A** | Needs winner detection                          |
| Sync both MCUs on game start                | **Person A** | Needs I2C and button handling                   |

## How Each Person Works in Parallel

### **Person A: I2C & Game Logic**

- Starts with I2C initialization and communication functions.
- Implements game state management (waiting, countdown, reaction, result).
- Manages synchronization between the two MCUs.
- Detects the winner and resets the game.

### **Person B: Buttons & LEDs**

- Sets up buttons and interrupts.
- Implements debounce mechanism to avoid false triggers.
- Handles the countdown sequence using LEDs.
- Displays winner/loser using LEDs.

## Collaboration Strategy

1. **Step 1:** Work independently on separate modules.
2. **Step 2:** Integrate button presses with game state transitions.
3. **Step 3:** Test button presses and countdown independently.
4. **Step 4:** Merge both parts and test full game logic with I2C communication.
