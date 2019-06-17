#Author: Benjamin Pontes
#Date last edited: 6/15/19
#CS104-01
                        #Program Description
"""
This program takes a weight and height inputed from the user and calculates the
    Body Mass Index (BMI) and checks if the calculated BMI is Underweight, 
    Normal, Overweight, or Obese. 
    Then prints out the the BMI and the appropiate BMI definition

"""


print ("Welcome to the BMI calaculator.\nYou will be asked to enter your weight and height.")

#weight_in_pounds : the users inputed weight in pounds    
weight_in_pounds = input("\nPlease enter your weight (in pounds) : ")

#Convert user inputed weight (weight_in_pounds) to float
weight_in_pounds = float(weight_in_pounds)

#Checking for negative weight entered : Exiting program if true
if(weight_in_pounds < 0):
    print("Error: the weight entered must be non-negative.")
    exit(0)

#height_in_inches : the users inputed height in inches  
height_in_inches = input("\nPlease enter your height (in inches) : ")

#Convert user inputed height (height_in_inches) to float
height_in_inches = float(height_in_inches)

#Checking for negative height entered : Exiting program if true
if(height_in_inches < 0):
    print("Error: the height entered must be non-negative.")
    exit(0)


#Convert (weight_in_pounds) from pounds to kilos. Stored in (weight_in_kilos)
weight_in_kilos = weight_in_pounds / 2.20462

#Convert (height in inches) from inches to meters. Stored in (height_in_meters)
height_in_meters = height_in_inches / 39.3701

#Calculating Body Mass index "BMI". (BMI = weight / height^2) where weight is in kilos and height is in meters
BMI = weight_in_kilos / (height_in_meters ** 2) 

#printing the calculated BMI
print("\nYour current BMI is:",BMI, end =" ")

#if the BMI is less than 18.5, the weight status is underweight
if(BMI < 18.5):
    print("which is Underweight.")

#if the BMI is >= 18.5 and below 25.0, the weight status is Normal; 
elif(18.5 <= BMI < 25.0):
    print("which is Normal.")

#if the BMI is >= 25.0 and below 30.0, the weight status is Overweight
elif(25.0 <= BMI < 30.0):
    print("which is Overweight.")

#else the BMI is > 30, and the the weight status is Obese
else:
    print("which is Obese.")


print("\nThank you. Goodbye\n")