# Rainbow-Six-Full-Cheat-Source (Overflow Cheats)
###### Just a long project which I stopped working on. Can bypass most anti-cheats with the only detection vector being the creation of a system thread. Project is very, very, messy

- **If your going to use this its not ready to paste, you'll need to setup a MySql database (unless you just delete the method of using a database and just put a hard coded password), preferably from Microsofts Azure service (ez and cheap).** 
     - Once thats done fill in your cridentials in the login.cpp file, and now you should have a working loader (make sure to set-up the tables correctly in the db).
     - Further explained here https://github.com/NMan1/Rainbow-Six-Cheat/issues/12#issuecomment-603886973
     - The program will crash if ran without a database (hence why I only added the cheat menu picture and not the loader since I deleted the database)
   
 <img src="https://thumb.gyazo.com/thumb/200_w/eyJhbGciOiJIUzI1NiJ9.eyJpbWciOiJfNTIyZGI0MTY4YzNiMDc5NTU3MDNiMmU2ZGJjYTg3ZDcifQ.C2XQp-Z_omuqxri5OzFya8ru8jOgIlj3ZwvvILJPupw-gif.gif"/>  
 
- **Features** 	
     - Fully functional loader (you must setup the database yourself)
     - Kernel Driver to handle the features
     - Shared Memory communication
     - No driver devices or objects created/used
     - In-Game menu (external)
     - **Visuals**
        - ESP
        - Player and Object Chams
        - No Flash
        - Adjustable Colors
        - Adjustable brightness

     - **Aimbot**
        - Silent Aim
        - No Recoil
        - Spread Control
        - Spread slider
        - Bullet/Damage Mulltipler (shot 18 bullets in 1 shot)
        - Customizable FOV and Toggle Keys (fov is range for aimbot to lock on)
        - Customizable aim selection and bone

     - **Misc**
        - Unlock All (Unlocks everything the game, including operators, every charms weapon skin elite skin etc.)
        - Speed changer
        - Weapon FOV changer
        - Player FOV changer
        - Adjustable sliders for FOV and speed
        - no clip

     - **Settings**
        - Customizable aimlock keys
        - Rage and legit settings toggle keys
        - Configs


**Compiling**

1. Right click on "OverflowR6.sln" in the first folder.
2. Open it in a text editor such as notepad++
3. Go to the 8th or so line where it says "OverflowDriver" with the path to the right. Change the directory path to the correct one on your system
4. Re-open or reload the solution in visual studio.
5. Compile in x64 Debug - make sure you have the windows driver development kit downloaded and configured correctly.
