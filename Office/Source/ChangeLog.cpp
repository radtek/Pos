/*

===========================================================================
Office Change log
===========================================================================
---------------------------------------------------------------------------
v4.12.1.1 (02/02/2010)
---------------------------------------------------------------------------
*	Fixed export to excel bug for Loyalty Audit report
*	Fixed member creation bug where total_spent was null 
	(prevents POS from using member, i.e., loading points, 
	earning points etc.)
---------------------------------------------------------------------------
v4.12.0.3 (30/7/2008)
---------------------------------------------------------------------------
*	Rolled forward changes in v4.11.0.7
*	Rolled forward changes in v4.11.0.5
---------------------------------------------------------------------------
v4.12.0.2 (22/7/2008)
---------------------------------------------------------------------------
*	Rolled forward changes in v4.8.0.7
---------------------------------------------------------------------------
v4.12.0.1 (15/05/2008)
---------------------------------------------------------------------------
*	First attemt at cashup facility
*	[Fix] Rolled forward issues fixed in v4.8.0.5
---------------------------------------------------------------------------
v4.12.0.0 (31/03/2008)
---------------------------------------------------------------------------
*	Added consumption by half hour, detailed and summary
---------------------------------------------------------------------------
v4.11.0.7 (30/7/2008)
---------------------------------------------------------------------------
*	Changed account export to initially select 1 month worth
---------------------------------------------------------------------------
v4.11.0.5 (15/7/2008)
---------------------------------------------------------------------------
*	Added premier functionality and account number to sale invoice export
*	[Fix] Truncation of bottom of 'Setup' form
---------------------------------------------------------------------------
v4.11.0.2 (15/4/2007)
---------------------------------------------------------------------------
*	[Fix] Loyalty audit report fixed to include both external and internal
	customer types
---------------------------------------------------------------------------
v4.11.0.1 (7/4/2007)
---------------------------------------------------------------------------
*	Store export date for invoices
*	[Fix] Show invoice date, not export date in invoice export
*	[Fix] Enumerating custormers for export failed under some conditions
---------------------------------------------------------------------------
v4.11.0.0 (10/12/2007)
---------------------------------------------------------------------------
*	Added export facility for Invoices to MYOB
*	Rolled forward changes from v4.8.0.4
*	Updated 'Sales By User' to include more totals and GPs for the summary
	report
*	Updated 'Loyalty Sales' to show profitable customers
*	Added more fields to the Loyalty 'Birthdays' report
*	[Fix] Anchor the Menu Editor 'Synch Sides' to bottom of screen
*	[Fix] Restoring deleted stock item where category or group is also deleted
	caused issues.
*	[Fix] Reuse member numbers looking in both member type 2 and 4
*	[Fix] Don't change member number if editing and tweak member number generator
---------------------------------------------------------------------------
v4.8.0.5 (11/5/2008)
---------------------------------------------------------------------------
*	[Fix] Update GetBillTendersFilter to utilize indexes and speed up
	Bill Tenders report
*	Bill payments report speed up with better index utilisation
---------------------------------------------------------------------------
v4.8.0.4 (10/12/2007)
---------------------------------------------------------------------------
*	Removed 'Distinct' from member filter for reporting
*	Changed 'Loyalty sales by Item' to use an inner join
*	Fixed 'Cashup' report to not show credit movement (Will still show in
	'Bill Tenders' and 'Bill Payments' reports
---------------------------------------------------------------------------
v4.8.0.3 (28/11/2007)
---------------------------------------------------------------------------
*	Removed redundant "Refresh" button from reports page
*	Fixed 'Bill Details' report to allow for sizes longer than 25 characters
---------------------------------------------------------------------------
v4.8.0.2 (23/10/2007)
---------------------------------------------------------------------------
*	Fix Cashup to work in Firebird.
*	Added Sales Summary by User and by category reports.
*	Added date selector for wages export.
*	Added Consumption by Category (Not for location report)
*	Added Profit field to Consumption + various tidy-ups
*	Revert to earlier version of Rave Reports to fix DataMirror issue.
*	Tidy loyalty reports
		- Rename "Points Earnt" to "Points Balance"
      - Consistent display of DOB
      - Rename column title to "Points Balance"
      - Add "Points Balance" to "Loyalty History (By Customer)" report
*	Remove "Days" column from wages report
---------------------------------------------------------------------------
v4.8.0.1 (12/10/2007)
---------------------------------------------------------------------------
*	Added 3rd party menu report.
*	Added consumption by third party code
---------------------------------------------------------------------------
v4.8.0.0 (9/10/2007)
---------------------------------------------------------------------------
*	Allow saving of a new menu.
*	Added weighed sizes and tare weight to item sizes for scales.
*	Changed menu editor file version from v4.10 to 4.80
---------------------------------------------------------------------------
v4.7.0.2 (3/10/2007)
---------------------------------------------------------------------------
*	Fix MYOB wages export to speed it up and export the ID field, not name.
*	UI updates.
---------------------------------------------------------------------------
v4.7.0.1 (26/9/2007)
---------------------------------------------------------------------------
*	Added loyalty audit reports.
---------------------------------------------------------------------------
v4.7.0.0 (21/9/2007)
---------------------------------------------------------------------------
*	Changed the version number.
---------------------------------------------------------------------------
v4.6.0.0 (3/9/2007)
---------------------------------------------------------------------------
*	Changed the version number.
---------------------------------------------------------------------------
v4.5.0.6 (16/7/2007)
---------------------------------------------------------------------------
*	Transfer time will now be set to the time of the transfer.
*	Output of the Stock Inquiry excel export is now in the same order as the
	report.
*	Importing a MicroEditor menu will now set GST values to correct default.
*	Utilise the new graphics for the splash and start screen.
*	Sort customers in the loyalty section by name. (TODO sort company as well)
*	Show user name rather than id in timeclock export form.
*	Wages report infinite-page issue resolved.
*	Export wages now removes users with '0' as their ID.
---------------------------------------------------------------------------
v4.5.0.5 (25/6/2007)
---------------------------------------------------------------------------
*	Set menu node (instead of NULL) to have focus after importing micro-editor menu.
*	Added 2 discount reports.
*	Updated cashup report to show payment groups.
*	Add grand total to chronological sales (all sales).
---------------------------------------------------------------------------
v4.5.0.4 (13/6/2007)
---------------------------------------------------------------------------
*	Attempt to fix invisible errors list in menu editor.
*	Wages report show hh:mm:ss in the time column instead of just hh:mm
*	Allow wages reports to be printed from the export page.
---------------------------------------------------------------------------
v4.5.0.3 (6/6/2007)
---------------------------------------------------------------------------
*	Payroll preview prior to export added.
*	Filter added to Tips and Vouchers report, and renamed.
*	Tidy the Stock Movement report and add stock group totals.
*	Prevent option names getting too long in the menu editor.
---------------------------------------------------------------------------
v4.5.0.2 (28/5/2007)
---------------------------------------------------------------------------
*	Added the ability to assign the date and a reference to a transfer batch.
*	Added a report to show stock transfers detailing the date and reference.
*	Check various menu property lengths in the 'Check' function.
*	Export payroll information to MYOB.
---------------------------------------------------------------------------
v4.5.0.1 (23/5/2007)
---------------------------------------------------------------------------
*	BUG: Bill Tenders report was not showing the note correctly. Was linked to
	the wrong data source.
*	BUG: Stock Receipt (Supplier Purchases) report now shows the Supplied unit
	instead of the stocktake unit.
*	BUG: Reset to '0' during stocktake would set the variance to the on hand
	value rather than the -ve on hand value.
*	Swapped the size and item name fields on the Menu Profitability report.
*	Only show items in the Menu Recipe report if they actually have a recipe.
*	When importing from the MicroEditor, set the Happy Hour price to be the
	same as the normal price.
*	Show warning when deleting a stock item with non-zero on hand, on order,
	pending sales/write-offs or is initialised for stocktake.
---------------------------------------------------------------------------
v4.5.0.0 (10/5/2007)
---------------------------------------------------------------------------
*	Wages report: added columns for clocked in time, number of hours and
	number of days. Now conforms to new database structure with key link.
	Excel export now has more info.
*	Updated wages export to new structure.
*	Fixed issue with invoice viewer where qty was count of keys, not sum
	of qty.
*	Fixed Invoice report to show invoice number correctly, sort correctly
	and do all grouping/summing in the report.
*	Added a debtor invoice detail report, accessible from the debtors
	section.
*	Added company name to more reports.
*	Added qty totals in consumption report.
---------------------------------------------------------------------------
v4.4.0.1 (12/04/2007)
---------------------------------------------------------------------------
*	Updated wages report. Links via a user settable 'Code' field that, in my
	opinion, should be dropped in favour of the key.

---------------------------------------------------------------------------
v4.4.0.0 (28/03/2007)
---------------------------------------------------------------------------
*	Changed version number and logo only.

---------------------------------------------------------------------------
v4.3.0.10 (13/03/2007)
---------------------------------------------------------------------------
*	Allow addition of debtors as they now need a member number and site ID
	for smart card loyalty.
*	Added a change log.

---------------------------------------------------------------------------
v4.3.0.9 (12/03/2007)
---------------------------------------------------------------------------
* Loyalty History Report has points loaded (purchased) added to the summary
  report. The details report has the field hidden as the data is unavailable
  from MenuMate with the intention to add it in v4.4. VIP loyalty not fixed!

---------------------------------------------------------------------------
v4.3.0.x (?)
---------------------------------------------------------------------------
* Menu check now looks for sizes not in the master list. It appears that the
  MicroEditor is introducing letter case changes for some sizes. UNRESOLVED!

===========================================================================

*/
