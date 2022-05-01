# FinancialPlanner
Personal Finance Planner <br>

Current Stable Version : **Version 0.2.6**

## For Final Users
There isn't a proper installation procedure yet. (The product is still in development)

## Future Objectives
- Deployment (installer wizard or something similar)
- Software Icon
- Allow currency customization
- System to manage multiple accounts [Edit]
- Category/Subcategory Manager [Edit]
- Subcategory Manager [Delete]
- Transactions operations [Edit] [Delete] done with popup forms
- Asset allocation new record [Edit] [Delete] done with popup forms
- Income/Expenses second plots page (other plots :) )
- Forecasting tab
- FIRE tab

## Version 0.3.0 Objectives
- Income/Expenses : Overview tab [monthly by subcat, yearly by subcat]
- Investments Tab : breakdown [5 years, 10 years, 25 years, 50 years] views
- fix: bugs hunting
- fix: when deleting account proceed in deleting all account related transactions (give an option to clear or not the data) and manage account accesses/conflicts after delete

### Version 0.2.7 Objectives
- Adding comments to transactions
- style: improve the looks of incomeExpenses details tab GUI
- style: adding savings, savings rate, investments variation in incomeExpenses details tab
- style: Popups instead of error text messages 
- Asset Allocation tab : basic GUI structure and backend logic (assets, location, currency exposure, ...)
- Dynamic Investment Classes Manager (with backend integration)

### Version 0.2.6 Features
- incomeExpenses tab: Adding a table for transfers
- fix: account plots fix
- investment tab fix: money transfer logic integration with investments(deposits/withdrawal)
- Allow Transfer between Accounts
- fix: little fixes

### Version 0.2.5 Features
- Fix : Return investments report without holes (filled at runtime)
- Fix : show yearly view in net worth control panel
- Control check function for new transactions
- Investments tab : GUI structure complete, plots created
- Transfer between accounts : basic logic

### Version 0.2.4 Features
- Investments Tab : plots and tabs upgrade
- Full GUI style improvement (also add headers to tables)
- New font (Roboto) available
- Calendar Widget for date selection (used in new transaction panel)
- Fix : not possible to add categories with the same name as an existing category
- Fix : not allowing the [delete] of Investments account and related issues
- Fix : Returns monthly reports without holes (filled at runtime)

### Version 0.2.3 Features
- Investments Tab : breakdown tab main view plots
- Investments Tab : breakdown tab GUI structure (not complete)
- Income/Expenses Detail tab: add column for investments returns
- Fix : including/excluding investments into pie charts in Income/Expenses

### Version 0.2.2 Features
- Investments Tab basic GUI
- Investments Logic Structure
- Investments : Details tab structure
- Backend System optimization (structure rethink)
- Fix : memory leaks
- Fix : performance issues

### Version 0.2.1 Features
- Income/Expenses : Overview tab [monthly by cat, yearly by cat]
- Investments, Asset Allocation, Forecasting, FIRE tabs added
- Major refactor and documentation

### Versions 0.1.x Features
- 0.2.0 : Delete accounts enabled, Category delete
- 0.1.9 : database upgrade, Accounts tracking and plots, Net Worth / IncomeExpenses / Account tabs link implementation
- 0.1.8 : Backend System upgrade (modularity, enabling sorting, ...), Income/Expenses (GUI, details tab), Category/Subcategory Manager
- 0.1.7 : Font/themes options, Backend System, System to manage multiple accounts, NW tab : GUI structure + backend link
- 0.1.6 : new windows/tabs (basic GUI structure), Source files structure upgrade and fix
- 0.1.5 : Style: Dark mode upgrade, Font: Whole Blender Pro Package imported
- 0.1.4 : Basic forecasting plot for compound interest calculator window, Font upgrade (Blender Medium Pro)
- 0.1.3 : Test window upgrade
- 0.1.2 : in App Docking Space for frames, Empty Test window
- 0.1.1 : Basic Compound Interest Calculator

## Libraries Used and other Dependencies
- vcpkg library manager <br>
- ImGui library (docking branch)
- ImPlot library for plotting
- jsoncpp to manage JSON files in c++
- Font list: Blender Pro, Roboto<br>
<div>Font made from <a href="http://www.onlinewebfonts.com">oNline Web Fonts</a> is licensed by CC BY 3.0</div>

## License and Copyright
� 2022 Alberto Foti. All Rights Reserved.
