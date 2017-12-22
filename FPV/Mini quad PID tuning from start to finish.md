> # Mini quad PID tuning from start to finish
# 小轴距四轴PID调参
> 原文见：[Rcgroups](https://www.rcgroups.com/forums/showthread.php?2439428-Mini-quad-PID-tuning-from-start-to-finish)，作者使用的是NAZA32、Cleanflight飞控

## Step One
> EDIT: Step Zero. If your motors are not completely symmetrical, create a cmix for your layout according to these instructions. Cmix is not required, but if you have a cmix, it will cause your P gains to be much closer together on the pitch and roll axis. This makes it easier to tune, since you can basically tune both axes at the same time, instead of having to do each one separately.

调参开始之前，先确定机架形式。相对来说X构型的机架因其对称性，调参相对容易些。因为结构的对称性会使滚转和俯仰方向的参数基本一致，这样就不用把这两个方向的参数调节分开来做了。

> Step One. I loaded the default PID values, then reduced D to 15 and I to 0.020. I believe that the first step should be to find a roughly correct range for P, without too much influence from I and D.

第一步，先依据默认的PID参数，把D和I值分别减小到15和0.02（针对本文原作者的飞控，这样做的目的是在调节P值时减小I和D值对P的影响，尤其是D值）。第一步调节的主要目的是寻找P的一个大概区间范围。

> The default value for both pitch and roll P is 4.0. I have a custom mixer set up for my motor layout, so that should help the P gain value be approximately the same on both axes. So I will be changing both pitch and roll P gain together, at least at first.

俯仰和滚转方向的默认P值大小为4.0，原作者使用的是对称机架，因此在初始调节时两个方向设置的P值大小基本一致。

> I hovered the quad and listened/looked for high frequency oscillations. There were none. I increased P to 8.0. None. I increased P to 12.0. Now I could just start to hear oscillations in the motors, but I didn't see them, at least not from LOS. I might be able to see them in the FPV camera. I increased P to 16.0. The oscillations were more pronounced, but still not out of control. Bear in mind that this is just during hover. Actual flight would be more likely to bring out oscillations.

解锁飞机进行定高，观察高频震荡，使用默认参数没有出现震荡现象。随后把P值增大到8.0，依旧没有高频震荡现象出现；将P值进一步增大到12.0，现在可以从电机的声响中听出震荡，但在视距(LOS)范围内看不出震荡；最后把P值进一步加大至16.0，震荡现象更加明显，但飞机仍然处于可控的范围。需要指出的是这是在悬停状态下观察的震荡状态，真实飞行中震荡现象会更容易出现。


> At this point, I have somewhat of a sense of what a flyable range of P values might be. I could hover the quad with reasonable control at P=4.0, and the quad didn't shake itself out of the air at P=16.0. This certainly is living up to PIDC#1's reputation for having a wide range of flyable values.

通过上面的测试，目前能够大概知道P值应当处在的一个范围：P值为4.0时，飞机是可控的，但P增大到16.0，飞机也不会因为P值太大崩掉。

> I set P down to 6.0 as a starting point. Tomorrow I'll take the quad out for some actual flying and work the numbers up further.

我将P值设成6.0，明天会带飞机到室外飞行进一步测试。

> ## Basic tuning of I gain 
## I值的基本调节
> Previously, I roughed-in a set of basically flyable PID values by starting with default values and testing them in hover. I set I and D to a relatively low value because I didn't want them excessively affecting my tuning of P. It's worth pointing out that, until you get to very high values of I, there tends to not be as much interaction between I and P as there is between D and P. So, to some degree, it's most important to start tuning P with a relatively low D gain. It's relatively safe to tune I up to close to its final value because that's unlikely to affect your ultimate P gain. P and D counteract each other to some degree, so if you start tuning P with too high of a D, you can end up in a state where both P and D are excessive and the copter flies poorly, but it's hard to tell exactly why. So it's important to find the right value for P with D at a relatively low value, then work up D and P together.

在P的初步调节测试中，在初始的PID值基础上进行修改获得了在悬停下可飞的参数。在一开始，将I和D值舍得相对较小以避免其对P的调节的影响。值得指出的是，除非把I值设定得特别大，I和P之间的相互作用会小于D和P之间的相互作用。因此，从某种程度上说，最重要的是现在较小的D值情况下调节P的大小。把I值调节到接近理想值问题不大，因为其基本不会影响最后的P值；但D和P之间相互影响较大，因此如果在D值较大的情况下就开始P的调节，可能最终得到的P和D值都较大，以致飞机飞得不行却又找不到什么原因。所以很重要的一条是在D值较小的条件下找到一个比较合适的P值，最后一起调节D和P的值。

> Once I have P at a basically flyable level, my next step is to check I. I find it distracting if the quad is wandering all over the place while I'm trying to finish tuning P to the nth degree, so I like to tune I after I get P in roughly the right place, then after I tune I, I go back to tuning P and D.

P的大概范围确定之后，就可以开始I的调节了。因为在进一步精确调节P的时候飞机晃来晃去挺难过的，在I调节得差不多以后再回头调节P和D的值。

> My starting value for I of 0.020 is based on past experience with the MultiWii PID controllers. Lacking that experience, I might try setting I to half its default value. I found that a value of I=0.020 was actually pretty good, whereas I expected it to be too low. In order to demonstrate the effect of low I gain, I turned I down to 0.010. Here's the results:
<iframe width="560" height="315" src="https://www.youtube.com/embed/EzUJP2Mk_xY" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

在初始设置中I的值是0.02，I在0.02时飞得比较好，为了说明I值太小的影响，把I值设成0.01，飞行效果如上的视频。

> One of my favorite tests for I gain is to punch out hard, then cut the throttle to about 25% and let the quad fall, then punch the throttle hard to arrest the fall. If the quad can maintain solid attitude during these maneuvers, I gain is probably not too low. In the video, I have the Blackbox trace for the pitch axis rcCommand up, so that you can see that I'm not moving the pitch stick at all during most of this maneuver. Watch how much the horizon changes during this maneuver, and also more subtly, during the level flight after the maneuver. There is a lot of drift.

我比较常用的一个测试I值的方法是先猛推油门，然后把油门收到1/4左右，再推油门避免飞机落地。如果飞机在这些操作中姿态基本不怎么变（不出现俯仰、滚转姿态变化），那I值就不偏小。在上面的视频里，Blackbox记录了飞行信息，可以看到在飞行过程中基本没怎么动俯仰操纵杆，但飞机的俯仰会发生改变，并且在后面的飞行中飞机出现隐隐约约的漂移。

> I've chosen to isolate the pitch axis because my experience is that, at least with mini quads, you tend to need more I gain on the pitch axis than the roll axis. This has nothing to do with the quad being slightly shorter than it is wide, because I've got a cmix to compensate for that. Rather, this has to do with the distribution of mass relative to the CG. The mass is much more centered on the roll axis, whereas it is more spread out on the pitch axis. Therefore, more force is needed to move the quad along the pitch axis than the roll axis. The cmix takes care of this to some degree, which is why we should find that our P gains are roughly the same on both pitch and roll, but cmix doesn't address the issue entirely.

这里选俯仰方向来进行说明的原因主要是（特别是像穿越机这样的小四轴），一般俯仰方向的I值要比滚转方向的更大，这和飞机的长宽不一样关系不大（因为原作者的飞机是X构型的），这主要是因为质量分布，在滚转方向上，质量分布更加对称，而在俯仰方向上飞机质心和形心相差得就比较大了。因此要维持稳定，需要在俯仰方向上施加更大的力矩。虽然相对来说X构型的机架会减缓这类的问题（表现在P值相差不大），但还是有所不同的。

> The next video shows I gain at 0.020. All other settings are the same.
<iframe width="560" height="315" src="https://www.youtube.com/embed/wgZD67PK5C0" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

上面的视频中，将I值设成了0.20，其它参数不变。

You should see that the quad holds attitude much better during the punchouts. It still tends to nose up a bit when the throttle is cut at the end of the punchout, and when the throttle is applied to arrest the fall. During normal forward flight, there is a small amount of wander, but it's much more noticeable in the playback than it is during flight. My gut feel is that this value is still a bit low, but getting close to the right value, at least for the pitch axis.

The bottom line is that if I notice my attitude changing when I didn't put in a stick input, or if I find that I'm constantly having to "nudge" the copter back onto course, that's probably an indication that I is too low on the axis in question.

EDIT TO ADD: Troubleshooting excessive I gain is a little harder because it's not as obvious (at least to me) when I is too high. Excessive I gain may sometimes manifest as low-frequency (2 Hz or slower, approximately) oscillations, but not always. Excessive I gain may also manifest as a feeling of "stiffness" in response to stick inputs. If the copter feels unresponsive on an axis, and you are sure that P is high enough and your rates are high enough, you may have excessive I on that axis. If you are already good at coordinating your turns, but you find that the copter seems to want to keep turning into a turn after you intend to end the turn, or after you end the turn, the copter seems to want to swing outwards--that may be excess I gain on the yaw axis, for example. Another example of possibly excessive I gain is that you finish a move and, maybe 1/4 second later, the copter adjusts its attitude without you moving the stick. This may be the I term trying to correct for previous error. The signature of this type of correction is that the copter will adjust its attitude, like it might pitch up slightly, and then it will stay there. It won't oscillate. The I term will unwind its error and then be satisfied and stop.
