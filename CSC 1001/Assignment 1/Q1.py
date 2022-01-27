# input the data
final_Account_Value = float(input("Enter the final account value: "))
annual_Interest_Rate = float(input("Enter the annual interest rate: "))
number_Of_Years = int(input("Enter the number of years: "))

# calculate
initial_Deposit_Amount = final_Account_Value / ((1+(annual_Interest_Rate/100))**number_Of_Years)

# output
print("The initial value is", initial_Deposit_Amount)
