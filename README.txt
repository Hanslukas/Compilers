 _____  _                    _         _____         _                                                        
/  ___|(_)                  | |       /  __ \       | |                                                       
\ `--.  _  _ __ ___   _ __  | |  ___  | /  \/  __ _ | |  ___                                                  
 `--. \| || '_ ` _ \ | '_ \ | | / _ \ | |     / _` || | / __|                                                 
/\__/ /| || | | | | || |_) || ||  __/ | \__/\| (_| || || (__                                                  
\____/ |_||_| |_| |_|| .__/ |_| \___|  \____/ \__,_||_| \___|                                                 
______             _ | |      _                           ___          _                            _         
| ___ \           | ||_|     | |                         / _ \        | |                          | |        
| |_/ /_   _  ___ | |__    __| |  ___ __      __ _ __   / /_\ \ _   _ | |_  ___   _ __ ___    __ _ | |_  __ _ 
|  __/| | | |/ __|| '_ \  / _` | / _ \\ \ /\ / /| '_ \  |  _  || | | || __|/ _ \ | '_ ` _ \  / _` || __|/ _` |
| |   | |_| |\__ \| | | || (_| || (_) |\ V  V / | | | | | | | || |_| || |_| (_) || | | | | || (_| || |_| (_| |
\_|    \__,_||___/|_| |_| \__,_| \___/  \_/\_/  |_| |_| \_| |_/ \__,_| \__|\___/ |_| |_| |_| \__,_| \__|\__,_|

This software accept a matemathical expression and it provides the result.
For example: (-2+5)^(-4)*3+2*(8-7*((7+2)-3))
Result is: −67,96

This software make use a pushdown model and use a scanner and a parser for to interpret the characters.


To compile using the make command.
For launch the software use: 
./simple_calc
This software manage only this operator:
-
+
*
/
^
%
If you write 00, the software provides an error.
The same for any other expression syntactically incorrect.

You can not use these expressions:
-(5)
+2-2
