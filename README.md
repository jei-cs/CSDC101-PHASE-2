# CSDC101-PHASE-2
Hello and welcome to my Phase 2 project in CSDC101 titled "Cocoyashi Arc: Keigan’s Journey"

# Overview

This is a continuation of our programming projects on text based rpg games. The first one was in the Bleachverse and was made only 
using conditional and if-else statements. Now phase 2 is set in the One piece universe specifically the cocoyashi arc. We were now
tasked to use arrays/vectors, functions, loops, and nested statements.

# Gameplay

It starts with Act 1 the prologue, giving some context on what the game is about. It follows Act 2 which is the training arc, you have 
48 turns to build up your 5 different stats, along with an extra skill gain stat. Act 3 then starts the Battle loop against the boss with
3 Phases. The game has two endings depending if you win or not.

# Design Notes
- Stat system: HP, ATK, END, EP, EP REGEN, & SELF DISCOVERY
- Skill unlocks: more SELF DISCOVERY = more SKILLS. Up to a maximum of 9 skills.
- Gate mechanics: Inspired by the 8-inner gates. Although only 2 gates are in the game. Boosts stats and allows ability uses. Can and will crash which will give debuffs if EP not managed properly.
- Gate crash penalties: If you run out of EP while a gate is activated your gate will crash and inflict debuffs.
- Cooldown tracking: Cooldowns are displayed whenever you pick to choose a skill in the battle loop.
- Story pacing: Hopefully I've put enough context and paced the story properly for you to understand the whole story of the game. I've put character dialogues and even immersive text like what the character is thinking/feeling.

# Training Demo
There is a training demo in the main menu initialization. It's just the whole structure of the trainingloop() function without the dialogues/story.

# Battle Demo
Also found in the main menu initialization. You can explore the 2 endings here. Although the final victory act (Act 4) is not included here. The stats are set to be forgiving enough so you can defeat the boss easily. (Spam the basic attack [1. Attack] if you want to lose intentionally. The boss has stupid high endurance anyway so you'd lose eventually without using skills)

# How to run:
1. Download the .cpp file
2. Compile with any C++ compiler then run.
3. Enjoy.

# Extra notes:
I added one new skill called "Art of Resistance" which buffs player endurance by 50%. I already implemented it 
before reading that there was no need to add new skills or items. It already functions properly, hopefully it helps
with your survivability.

The code is kind of spaghetti coded. This is with the help of sleep deprivation and short memory retention. :/

