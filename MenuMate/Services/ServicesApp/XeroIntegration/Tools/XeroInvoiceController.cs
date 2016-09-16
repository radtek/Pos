using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xero.Api.Core.Model;
using Xero.Api.Core.Model.Types;
using Xero.Api.Core.Model.Status;
using System.Xml;
using AccountingIntegration.Xero;

namespace AccountingIntegration.Tools
{
    public class XeroInvoiceController
    {
        private static readonly object syncRoot = new Object();
        private static volatile XeroInvoiceController _instance;

        private XeroInvoiceController()
        {

        }

        public static XeroInvoiceController Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new XeroInvoiceController();
                    }
                }

                return _instance;
            }
        }


        public CreditNote CreateCreditNote(XeroInvoice invoice)
        {
            try
            {
                var invoiceToCreate = new CreditNote();
                invoiceToCreate.Contact = GetInvoiceContact(invoice.Contact);
                invoiceToCreate.Type = GetCreditNoteType(invoice.Type);
                invoiceToCreate.Number = invoice.Number;
                invoiceToCreate.Reference = invoice.Reference;
                invoiceToCreate.LineAmountTypes = GetInvoiceLineAmountType(invoice.LineAmountType);
                invoiceToCreate.Date = GetDate(invoice.Date);
                invoiceToCreate.DueDate = GetDate(invoice.DueDate);
                invoiceToCreate.LineItems = GetCreditLineItems(invoice.Items);
                invoiceToCreate.Status = GetStatus(invoice.Status);
                return invoiceToCreate;
            }
            catch (Exception exception)
            {
                throw new XeroIntegrationException(exception.Message);
            }
        }

        public Invoice CreateInvoice(XeroInvoice invoice)
        {
            try
            {
                var invoiceToCreate = new Invoice();
                invoiceToCreate.Contact = GetInvoiceContact(invoice.Contact);
                invoiceToCreate.Type = GetInvoiceType(invoice.Type);
                invoiceToCreate.Number = invoice.Number;
                invoiceToCreate.Reference = invoice.Reference;
                invoiceToCreate.LineAmountTypes = GetInvoiceLineAmountType(invoice.LineAmountType);
                invoiceToCreate.Date = GetDate(invoice.Date);
                invoiceToCreate.DueDate = GetDate(invoice.DueDate);
                invoiceToCreate.LineItems = GetLineItems(invoice.Items);
                invoiceToCreate.Status = GetStatus(invoice.Status);
                return invoiceToCreate;
            }
            catch (Exception exception)
            {
                throw new XeroIntegrationException(exception.Message);
            }
        }

        private InvoiceStatus GetStatus(string inStatus)
        {
            var status = InvoiceStatus.Draft;

            switch (inStatus)
            {
                case "DRAFT":
                    status = InvoiceStatus.Draft;
                    break;

                case "SUBMITTED":
                    status = InvoiceStatus.Submitted;
                    break;

                case "DELETED":
                    status = InvoiceStatus.Deleted;
                    break;

                case "AUTHORISED":
                    status = InvoiceStatus.Authorised;
                    break;

                case "PAID":
                    status = InvoiceStatus.Paid;
                    break;

                case "VOIDED":
                    status = InvoiceStatus.Voided;
                    break;
            }
            return status;

        }

       private List<LineItem> GetLineItems(XeroInvoiceItems inXeroInvoiceItems)
        {
            List<LineItem> result = new List<LineItem>();
            foreach (var item in inXeroInvoiceItems.Item)
            {
                result.Add(GetLineItem(item));
            }
            return result;
        }

        private LineItem GetLineItem(XeroInvoiceItem item)
        {
            LineItem result = new LineItem();
            result.AccountCode = item.AccountCode;
            result.Description = item.Description;
            result.UnitAmount = Convert.ToDecimal(item.UnitAmount);
            result.TaxAmount = Convert.ToDecimal(item.TaxAmount);
            result.Quantity = Math.Abs(Convert.ToDecimal(item.Qty));
            return result;
        }

        private List<LineItem> GetCreditLineItems(XeroInvoiceItems inXeroInvoiceItems)
        {
            List<LineItem> result = new List<LineItem>();
            foreach (var item in inXeroInvoiceItems.Item)
            {
                result.Add(GetCreditLineItem(item));
            }
            return result;
        }

        private LineItem GetCreditLineItem(XeroInvoiceItem item)
        {
            LineItem result = new LineItem();
            result.AccountCode = item.AccountCode;
            result.Description = item.Description;
            result.UnitAmount = Math.Abs(Convert.ToDecimal(item.UnitAmount));
            result.TaxAmount = Math.Abs(Convert.ToDecimal(item.TaxAmount));
            result.Quantity = Math.Abs(Convert.ToDecimal(item.Qty));
            return result;
        }

        private DateTime? GetDate(string inDate)
        {
            DateTime date;
            try
            {
                date = DateTime.ParseExact(inDate, "dd/MM/yyyy HH:mm:ss", null);
            }
            catch (Exception)
            {
                date = DateTime.Now;
            }
            return date;
        }

        private LineAmountType GetInvoiceLineAmountType(string inLineAmountType)
        {
            var lineAmountType = LineAmountType.NoTax;
            switch (inLineAmountType)
            {
                case "0":
                    lineAmountType = LineAmountType.Exclusive;
                    break;
                case "1":
                    lineAmountType = LineAmountType.Inclusive;
                    break;
                case "2":
                    lineAmountType = LineAmountType.NoTax;
                    break;
            }
            return lineAmountType;
        }

        private CreditNoteType GetCreditNoteType(string inCreditNoteType)
        {
            var invoiceType = CreditNoteType.AccountsReceivable;
            switch (inCreditNoteType)
            {
                case "ACCPAYCREDIT":
                    invoiceType = CreditNoteType.AccountsPayable;
                    break;
                case "ACCRECCREDIT":
                    invoiceType = CreditNoteType.AccountsReceivable;
                    break;
            }
            return invoiceType;
        }

        private InvoiceType GetInvoiceType(string inInvoiceType)
        {
            var invoiceType = InvoiceType.AccountsReceivable;
            switch (inInvoiceType)
            {
                case "ACCPAY":
                    invoiceType = InvoiceType.AccountsPayable;
                    break;
                case "ACCREC":
                    invoiceType = InvoiceType.AccountsReceivable;
                    break;
            }
            return invoiceType;
        }

        private Contact GetInvoiceContact(XeroContact xeroContact)
        {
            return new Contact()
            {
                Name = xeroContact.Name
            };
        }

        public List<Payment> GetInvoicePayments(XeroInvoicePayments inXeroInvoicePayments, Invoice inInvoice)
        {
            List<Payment> result = new List<Payment>();
            foreach (var payment in inXeroInvoicePayments.Item)
            {
                 result.Add(GetInvoicePayment(payment, inInvoice));
           
           }
            return result;
        }

        private Payment GetInvoicePayment(XeroInvoicePayment payment, Invoice inInvoice)
        {
            Payment result = new Payment();
            result.Account = new Account()
            {
                Code = payment.AccountCode
            };

            result.Invoice = inInvoice;
            result.Date = Convert.ToDateTime(inInvoice.Date);
            if (result.Amount < 0)
            {
                result.Type = PaymentType.AccountsReceivableCredit;
            }
            result.Amount = Math.Abs(Convert.ToDecimal(payment.UnitAmount));
            return result;
        }

        public List<Payment> GetCreditNotePayments(XeroInvoicePayments inXeroInvoicePayments, CreditNote inCreditNote)
        {
            List<Payment> result = new List<Payment>();
            foreach (var payment in inXeroInvoicePayments.Item)
            {
                    result.Add(GetCreditNotePayment(payment, inCreditNote));
          
            }
            return result;
        }

        private Payment GetCreditNotePayment(XeroInvoicePayment payment, CreditNote inCreditNote)
        {
            Payment result = new Payment();
            result.Account = new Account()
            {
                Code = payment.AccountCode
            };
            if (result.Amount < 0)
            {
                result.Type = PaymentType.AccountsReceivableCredit;
            }
            result.Amount = Math.Abs(Convert.ToDecimal(payment.UnitAmount));
            result.CreditNote = inCreditNote;
            result.Date = Convert.ToDateTime(inCreditNote.Date);

            return result;
        }
    }
}
