# MCTSGladiator
A StarCraft bot applied MCTS for tactical decision-making.

## Demo (Youtube)
[![Demo Video on Youtube](http://img.youtube.com/vi/kTcIdG74ys8/0.jpg)](http://www.youtube.com/watch?v=kTcIdG74ys8)

## Abstract
In this thesis, we apply Monte Carlo tree search for tactical decision-making in StarCraft, which is about controlling units to combat with opponent’s units in real-time strategy games. MCTS can use less expert knowledge to achieve high performance and adaptive plays. And, for real-time games, MCTS can stop searching at any time to return the current best move.

Applying UCT considering duration, which considers durative moves in UCT, we can implement a simulator for MCTS with reasonable game abstraction. In experiments, we found the value for exploration constant and the number of max children for nodes in tree first. Then we tested different number of units and different kinds of units, including 4/8/16/32/50 units and pure ranged/pure melee/mixed units’ compositions.

First, the results showed that the best evaluation function is HPD, which was defined as the difference of total hitpoints of both sides, rather than other functions considering units’ damage. It’s not wrong to consider more information of units, but perhaps it’s suitable to use them that way. Second, MCTS did more well on ranged units than melee units. Because we needed better performance of simulator, ignoring unit collisions seemed to prevent it from finding good strategy for melee units. And third, MCTS couldn’t handle too much units.

This research prevails the applicability of MCTS to real-time strategy. If other researchers can afford more computational resources, it is certain that this method would show better performance and results. On the other hand, for game developers, they can use this method to implement a more interesting and better built-in AI of their RTS games.

## Thesis (Chinese)
[Download](https://github.com/NagaChiang/MCTSGladiator/blob/master/Applying%20Monte%20Carlo%20tree%20search%20for%20tactical%20decision-making%20in%20Starcraft.pdf)
