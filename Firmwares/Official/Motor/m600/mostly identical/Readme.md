# Information after Diffing:

hex address = value == effect/target-effect

**Fully Charged voltage**
0768 = 29 == 41( 36v firmwares)
0768 = 30 == 48 ( 43v firmwares)
0768 = 35 == 53 ( 48v firmwares)


**Low voltage level:**

076c = 2A  == 42v
076c = 20 == 32v
076c = 27 == 39v

**Normal/Nominal Voltage setting:**

0770 = 24 == 36v
0770 = 2B == 43V
0770 = 30 == 48v


**Max current setting:**

0774 = 0a == 10A
0774 = 0d == 13A
0774 = 0f == 15A
0774 = 12 == 18A
0774 = 14 == 20A

**Max current setting 2:**
somewhere around 11b00, this whole section differs between firmwares with different voltage setting

24 24 == 36v
2B 2B == 43V
30 30 == 48v
