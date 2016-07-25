CREATE TABLE ORDERSTRANSC
( 
    p_key        INTEGER NOT NULL PRIMARY KEY, 
    
    transc_no  INTEGER NOT NULL,
	date_time TIMESTAMP 

	PRIMARY KEY (p_key)
);

CREATE TABLE PAYMENTTYPETRANSC
( 
    p_key              INTEGER NOT NULL, 
    	
	payment_name VARCHAR(30),
	properties        NUMERIC(15,4),
	exchange_rate NUMERIC(15,4),
	display_order   INTEGER,
	percent_Adjust NUMERIC(15,4),
	amount_adjust NUMERIC(15,4),
	adjust_reason VARCHAR(30),
	group_number INTEGER,
	taxt_rate         NUMERIC(15,4),
	round_to         NUMERIC(15,4),

	PRIMARY KEY (p_key)
 );

CREATE UNIQUE INDEX PAYMENT_TYPE_TRANSC_GROUP_INDEX ON PAYMENTTYPETRANSC (group_number);

 CREATE TABLE ORDERPAYMENTTYPETRANSC
( 
    p_key                           INTEGER NOT NULL, 
    	    
    order_payment_type_fk INTEGER NOT NULL,        
    order_fk                       INTEGER NOT NULL,

	PRIMARY KEY (p_key)
    FOREIGN KEY (orderPaymentTypeFK) REFERENCES PAYMENTTYPETRANSC(order_payment_type_fk) ON DELETE CASCADE ON UPDATE CASCADE, 
    FOREIGN KEY (orderFK)                    REFERENCES ORDERSTRANSC(order_fk) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE ORDERITEMSTRANSC
( 
    p_key                   INTEGER NOT NULL PRIMARY KEY, 
    
    menu_price           NUMERIC(15,4) NOT NULL,
    sale_price             NUMERIC(15,4) NOT NULL,
    tax_inclusive          T_TRUEFALSE   DEFAULT 'F',
    tax                        NUMERIC(15,4) NOT NULL,
    tax_exempt            T_TRUEFALSE   DEFAULT 'F',
    zero_tax_rated       T_TRUEFALSE   DEFAULT 'F',
    service_charge       NUMERIC(15,4) NOT NULL,
    service_charge_tax NUMERIC(15,4) NOT NULL,
    
    is_free                   T_TRUEFALSE DEFAULT 'F',
         
    orders_fk              INTEGER NOT NULL,
    
	PRIMARY KEY (p_key)
    FOREIGN KEY (ordersFK) REFERENCES ORDERSTRANSC(orders_fk) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE ORDERSIDESTRANSC
( 
    p_key                   INTEGER NOT NULL PRIMARY KEY, 
    
    menu_price           NUMERIC(15,4) NOT NULL,
    sale_price             NUMERIC(15,4) NOT NULL,
    tax_inclusive          T_TRUEFALSE   DEFAULT 'F',
    tax                        NUMERIC(15,4) NOT NULL,
    tax_exempt            T_TRUEFALSE   DEFAULT 'F',
    zero_tax_rated       T_TRUEFALSE   DEFAULT 'F',
    service_charge       NUMERIC(15,4) NOT NULL,
    service_charge_tax NUMERIC(15,4) NOT NULL,
    
    is_free                  T_TRUEFALSE DEFAULT 'F',
    
    order_items_fk      INTEGER NOT NULL,
     
    PRIMARY KEY (p_key)
    FOREIGN KEY (orderItemsFK) REFERENCES ORDERITEMSTRANSC(order_items_fk) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE ORDERITEMSTAXTRANSC
( 
    p_key           INTEGER NOT NULL PRIMARY KEY, 
    	    
    order_tax_fk     INTEGER NOT NULL,        
    order_items_fk  INTEGER NOT NULL,
     
    PRIMARY KEY (p_key)
    FOREIGN KEY (orderTaxFK)   REFERENCES ORDERITEMSTAX(order_tax_fk)      ON DELETE CASCADE ON UPDATE CASCADE, 
    FOREIGN KEY (orderItemsFK) REFERENCES ORDERITEMSTRANSC(order_items_fk) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE ORDERSIDESTAXTRANSC
( 
    p_key           INTEGER NOT NULL PRIMARY KEY, 
    	    
    order_tax_fk    INTEGER NOT NULL,        
    order_sides_fk  INTEGER NOT NULL,
     
    PRIMARY KEY (p_key)
    FOREIGN KEY (orderTaxFK)   REFERENCES ORDERITEMSTAX(order_tax_fk)      ON DELETE CASCADE ON UPDATE CASCADE, 
    FOREIGN KEY (orderSidesFK) REFERENCES ORDERSIDESTRANSC(order_sides_fk) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE ORDERITEMSDISCOUNTTRANSC
( 
    p_key             INTEGER NOT NULL PRIMARY KEY, 
    	    
    order_discount_fk INTEGER NOT NULL,        
    order_items_fk    INTEGER NOT NULL,
     
    PRIMARY KEY (p_key)
    FOREIGN KEY (orderDiscountFK) REFERENCES DISCOUNTTRANSC(order_discount_fk) ON DELETE CASCADE ON UPDATE CASCADE, 
    FOREIGN KEY (orderItemsFK)    REFERENCES ORDERITEMSTRANSC(order_items_fk)      ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE ORDERSIDESDISCOUNTTRANSC
( 
    p_key                   INTEGER NOT NULL PRIMARY KEY, 
    	    
    order_discount_fk INTEGER NOT NULL,        
    order_sides_fk     INTEGER NOT NULL,
     
    PRIMARY KEY (p_key)
    FOREIGN KEY (orderDiscountFK) REFERENCES DISCOUNTTRANSC(order_discount_fk) ON DELETE CASCADE ON UPDATE CASCADE, 
    FOREIGN KEY (orderSidesFK)    REFERENCES ORDERSIDESTRANSC(order_sides_fk) ON DELETE CASCADE ON UPDATE CASCADE 
);

CREATE TABLE DISCOUNTTRANSC
( 
    p_key                     INTEGER NOT NULL PRIMARY KEY, 
    	    
    percentage             NUMERIC(15,4) NOT NULL,
    amount                  NUMERIC(15,4) NOT NULL,
	discount_type         INTEGER NOT NULL,
	rounding                NUMERIC(15,4) NOT NULL,
	category_key          INTEGER NOT NULL,
	properties               NUMERIC(15,4) NOT NULL,
	priority                   INTEGER NOT NULL,
	discount_id 	    	 INTEGER NOT NULL,
	appeareance_order INTEGER NOT NULL,
	members_only        T_TRUEFALSE DEFAULT 'F',
	discount_group       INTEGER NOT NULL,
	
	PRIMARY KEY (p_key)
);