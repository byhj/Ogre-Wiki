# Ogre-Wiki
OgreWiki Sample
Welcome to the Ogre Wiki Code by byhj!

1.Introduction
============================================================================================================================
This Project is The Ogre Wiki Code
http://www.ogre3d.org/tikiwiki/tiki-index.php
Code Include:
     Tutorials
              Basic Tutorials 
              Intermediate Tutorials
              Mad Marx Tutorials
              In Depth Tutorials 
    CodeBank:

=============================================================================================================================

2.Requirements
=============================================================================================================================
  1. Visual Studio 2012 or 2013 (and some knowledge on how to build, run, and debug C++ projects)
  2. OgreSDK1.9 or Ogre Source code build by yourself
  
=============================================================================================================================

3.How to Buil
=============================================================================================================================
 1. Make sure you develop environment is right.
 2. You can build the environment by yourself according your target.
 3.Project Organization:
		  |Root
              |-- include 
              |-- src
              |-- media
              
 Notice: I use the Visual Studio project props file to share the project setting.
         The output dir:
              |-- bin(Execute Dir) 
              |-- temp(temp file we possibly not need) 

=============================================================================================================================

4.How to Run
=============================================================================================================================
Open OpenGL-Bluebook.sln
  1. Right click on the project and set is as your startup project.
  2. Enter the exe Dir, run the exe to view the render output.
  
Notice:You should set the OGRE_HOME to system environment variables path to your Ogre1.9 dir!
  
