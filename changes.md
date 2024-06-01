# Changes made since last release 

**1.** Bevel has a new sharp bevel mode that is in both stand alone bevel and GEGL Effects.
Sharp Bevel mode uses a unique algorithm that makes an awesome new bevel effect.
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/e71485aa-fa0c-451f-b5f3-dd221be69ed6)

**2.** Gimp 2.99/3 users will see GEGL Effects in their menu under Filters/Text Styling (2.10 is incapable of this)

## Tiny changes and emergency fixes made in the July 22 2023 series that do not qualify as a new checkpoint release because there are no new features, only internal modifications that most will not notice.

**3.** August 1st 2023 minor update allows Glass on Text to be on top of Bevel. This was a simple node rearrangement and does not qualify as a new release.

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/3e54daac-8d56-41c5-a168-eddc750af25b)

**4**  August 1st 2024 renaming of blend modes.  Normal blend modes are now called "Normal" instead of  "over".

**5** November 2023 Bevel as a stand alone plugin updated to get a bunch of new modes. GEGL Effects does not have access to them 

**6** March 10 2024 the namespace (gegl:) was replaced with (lb:) which means ALL PRESETS BROKE but GIMPs team made it mandatory that I do this.  (gegl:) namespace presets can still be imported from text files and work

**7** March 10 2024  a now retracted release of GEGL Effects debuted with a checkbox to solve the clipping bug at the expense of another bug

**8**  April 5th 2024  Inner Glow as a stand alone plugin updated  and got a bunch of new options. GEGL Effects can't access these changes

**9** May 2 2024 the March 10 2024 checkpoint was recalled due to its only new GUI option being removed. GEGL Effects is now back on the July 22 2023 checkpoint

**10** May 4 2024  Inner Glow the stand lone plugin's previous update was tweaked and now Inner Glow no longer works in adjustment layer groups in its default mode. However Inner Glow does work in adjustment layer groups in its alternative modes. This is to prevent a crash in GEGL Effects. GEGL Effects was not effected by this other then not crashing when an image upload preset was switched.

**11** May 4th 2024 GEGL Effects updated again to solve the delayed color update bug and clipping problem once and for all (hopefully) It no longer needs a checkbox to do this either. The bug was solved by modifying code in zzstrokebevelimage.c; the advance dropshadow like operation. 

**12** May 21th-May 28th 2024 - 33 plugins were updated so they no longer crash when closing on GIMP 2.99.19. This includes GEGL Effects. No changes were made to GEGL Effects though.
