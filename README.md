# 🤖 AlphaBot2 Autonomous Line Follower with Obstacle Avoidance

> 👩‍💻 Built by **Sharma Helee Bharatkumar** as part of the e-Yantra EEP-01 course.

Twelve weeks ago, I wrote my very first line of embedded code. Today, I have a robot that follows a line, spots obstacles in its way, and decides what to do next, all on its own.

This repository is the story of that journey. Every concept, every activity, and every line of code in this course was a stepping stone toward one final goal: the capstone robot.

---

## 🎯 What this robot does

My AlphaBot2 can:

- **Follow a line** using its IR sensor array and keep itself on track
- **Detect obstacles** in its path using its obstacle sensors
- **Make decisions in real time**: stop, go around the obstacle, and find its way back to the line
- **Show what it's thinking** through the onboard NeoPixel LEDs

No remote control, no hand-holding. Just sensors, code, and logic.

---

## 🧰 Hardware

| Part | Details |
|------|---------|
| Robot platform | Waveshare AlphaBot2 |
| Controller | Arduino Uno (ATmega328P) |
| Line sensing | IR line-tracking sensor array |
| Obstacle sensing | IR obstacle avoidance sensors |
| Motors | 2x DC motors via the on-board motor driver (PWM speed control) |
| Indicators | 4x onboard WS2812B RGB LEDs (NeoPixel) |

---

## 🧠 How it works

The robot runs a simple loop, over and over, very fast:

1. **Read the line sensors** to see where the line is under the robot
2. **Work out the error**: is the robot drifting left or right?
3. **Adjust motor speeds** to steer back toward the line
4. **Check for obstacles** on every single loop
5. **If something is in the way**, run the avoidance routine, then search for the line again and carry on

---

## 💡 Status lights (NeoPixel)

The onboard RGB LEDs tell me what the robot is doing at a glance:

| Color | Meaning |
|-------|---------|
| 🟢 Green | Following the line |
| 🔴 Red | Obstacle detected |
| 🟡 Yellow | Avoiding the obstacle |
| 🔵 Blue | Searching for the line |

---

## 📌 Pin mapping

| Component | Arduino Uno pin |
|-----------|-----------------|
| Left motor PWM | ... |
| Right motor PWM | ... |
| Line sensor array | ... |
| Obstacle sensor (left) | ... |
| Obstacle sensor (right) | ... |
| NeoPixel data (4 LEDs) | ... |

---

## 📚 What I learned

- Writing embedded C/C++ on the Arduino Uno
- Working with GPIO, timers, and interrupts
- Reading and calibrating sensors
- Controlling motors with PWM
- Driving addressable RGB LEDs (NeoPixel)
- Building real-time decision logic that the robot runs by itself

---

## 📁 Repository structure

```
/week-01 ... /week-12    Weekly activities, each building on the last
/capstone                Final line follower + obstacle avoidance code
/images                  Photos and demo GIFs
README.md                You are here
```

Each week's folder has its own README explaining that week's activity and how to run the code.

---

## 🚀 Getting started

1. Mount the Arduino Uno on the AlphaBot2 base
2. Install the [Arduino IDE](https://www.arduino.cc/en/software)
3. Install the **Adafruit NeoPixel** library:
   *Sketch → Include Library → Manage Libraries → search "Adafruit NeoPixel" → Install*
4. Clone this repo:
   ```bash
   git clone https://github.com/helee03112007/eep-01-alphabot2-line-follower.git
   ```
5. Open `capstone/capstone.ino` in the Arduino IDE
6. Go to **Tools → Board → Arduino Uno** and select the correct port
7. Click **Upload**, place the robot on a line, and watch it go

---

## 🎬 Demo

![Robot demo](images/demo.gif)

---

## 🙏 Acknowledgements

Built as part of the **e-Yantra EEP-01** course by IIT Bombay. Thanks to the e-Yantra team for turning "what is a microcontroller?" into "I built a robot."

---

## 📬 Contact

Made by **Sharma Helee Bharatkumar** ([@helee03112007](https://github.com/helee03112007)). Feel free to open an issue or reach out if you have questions or ideas.

---

## 📄 License

This project is licensed under the MIT License. See the `LICENSE` file for details.
© 2026 Sharma Helee Bharatkumar
