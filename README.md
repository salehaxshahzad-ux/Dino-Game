
# 🦖 C++ ASCII Dino Runner Game

> *Saved during late-night exam prep, built during summer break, debugged through sheer willpower.*

A lightweight, terminal-based recreation of the classic Google Chrome Offline Dinosaur game built completely in pure C++ using standard Windows Console APIs (`windows.h`) and ASCII art.

---

## 📸 Demo & Features

* **Pure ASCII Graphics:** Custom-designed Dino (`@`) and Cactus (`|`) sprites tailored for standard OEM terminal window encodings.
* **Seamless Animation & Physics:** Smooth jumping mechanics and leg-walking animation with proper frame delays to prevent flickering.
* **Accurate Collision Detection:** Fine-tuned X/Y hitbox detection for clearing hurdles.
* **Persistent High Scores:** Writes player scores directly to `score.txt` and sorts the **Top 10 High Scores** using dynamic pointer allocations.
* **In-Game Controls:** Includes jump, pause/resume, high score reader, and menu navigation.

---

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **`SPACEBAR`** | Jump over Cactus hurdles |
| **`P`** | Pause / Resume Game |
| **`ESC`** | Exit to Main Menu |

---

## 🛠️ Project Structure & Technical Fixes

During development, several real-world console programming challenges were solved:
1. **Encoding Glitches:** Replaced extended Unicode block characters with standard ASCII characters (`@`, `#`) to resolve character decoding errors across standard Dev-C++ / TDM-GCC compilers.
2. **Buffer Trails & Artifacts:** Implemented explicit coordinate tracking with `gotoxy()` to wipe out trailing screen artifacts during high-speed movement.
3. **Stream Memory Safety:** Implemented manual pointer allocations for reading and bubble-sorting file streams to avoid file buffer locks.

---

## 🚀 How to Run

1. Clone this repository:
   ```bash
   git clone [https://github.com/your-username/cpp-dino-runner.git](https://github.com/your-username/cpp-dino-runner.git)
