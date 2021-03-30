# Bookshop_database is a small app that imitates Bookshop with mySQL database.
 
 # Functionality:
    1. adding admins with special priviligies
    2. adding, deleting, managing books and customers
    3. customer registration
    4. user verification by login and password:
        -unique logins, 
        -password encrypted with sha256
        -prevention from SQL injection
    5. making orders by customers
    6. counting bestsellers
    7. counting income
  
# Database:
  1. Access is set default in main.cpp
  2. Empty database to import is stroed: Database/EMPTY_bookshop.sql
  3. Database with some filled records:  Database/FILLED_bookshop.sql,
       -logins and passwords for those accounts: Database/login_password.txt
        
 Project works on Windows with Code::Blocks, configuration C::B: https://www.youtube.com/watch?v=ssmyFWoZjLk
