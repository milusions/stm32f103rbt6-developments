# STM32 Bare-Metal & HAL Tutorial Series 🚀

Welcome to the official companion repository for the **Milusions STM32 Video Tutorial Series**. 

This repository contains all the source code, register configurations, and project files used in the videos. The goal of this series is to demystify embedded systems by taking you from standard HAL (Hardware Abstraction Layer) programming down to the bare-metal register level.

📺 **[Watch the full video playlist here](#)** *(Add your YouTube playlist link here)*

## 📂 Repository Structure

The repository is organized by episode/topic. Each folder contains a standalone project.

* **`Ep01_BareMetal_GPIO/`** * Source code for controlling GPIO pins without HAL.
  * Covers RCC (Clock Enable), CRL/CRH (Mode Configuration), and BSRR/ODR (Pin Toggling).
* **`Ep02_HAL_GPIO_Blink/`** * The standard STMicroelectronics HAL equivalent of a GPIO write/toggle.
  * Demonstrates the use of `HAL_GPIO_WritePin` and `HAL_Delay`.
* *(Add future episodes here...)*

## 🛠️ Hardware & Tools Used

To follow along with these tutorials, you will need:
* **Microcontroller:** STM32F1 Series (e.g., STM32F103C8T6 "Blue Pill" or similar Nucleo board).
* **Programmer:** ST-Link V2.
* **IDE:** [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (Free from STMicroelectronics).

## 🧠 Key Concepts Covered
* **Memory Mapping:** Understanding how physical hardware addresses (`0x40021000`, etc.) translate to C pointers.
* **Bitwise Operations:** Shifting and masking bits (`|=`, `&= ~`) to modify registers safely.
* **The Bus Architecture:** Understanding the AHB and APB bus systems.

## 🚀 How to Use This Code

1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/YourUsername/YourRepoName.git](https://github.com/YourUsername/YourRepoName.git)
