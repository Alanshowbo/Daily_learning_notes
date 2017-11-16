## Chapter 02--Using Variables and Parsing
+ ctor Tab Tab，构造函数
+ protected，衍生类（override）
+ internal，仅在本项目中可用
+ get,set
+ int.TryParse(m string, out outset)，返回true/false，true则赋值outset
+ string.concat
+ Conole.Writeline($"Your new number is {usernumber}.")

## Chapter 03--While loops and Complex logic
+ if( () && () || () )
+ response !=Null && response.Length >0, response?.Length >0
```
//NULL 条件运算符
int? length = customers?.Length; // null if customers is null   
Customer first = customers?[0];  // null if customers is null  
int? count = customers?[0]?.Orders?.Count();  // null if customers, the first customer, or Orders is null
```
+ string.ToLower().FirstOrDefault() == 'y'
+ Math.Ceiling()向上进位取整，Math.Floor()向下进位取整

## Chapter 04--Classes Functions and Regions
+ {get; private set;}

## Chapter 05--Windows Forms and Event Handlers
+ this.CE_btn.Click += new System.EventHandler(this.CE_btn_Click);

## Chapter 06--Creating a Caculator - Manipulating string
+ string.Empty
+ Text.Focus()//TextBox
+ Text.SelecntionStart()//TextBox
+ string.Remove(index, num); //return new string
+
