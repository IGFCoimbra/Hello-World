package db

import (
	"database/sql"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"strings"
	"time"

	_ "github.com/lib/pq"
)

func InitDatabase() *sql.DB {
	db, err := getDatabase()
	if err != nil {
		log.Fatal(err)
	}
	if err = db.Ping(); err != nil {
		log.Fatal(err)
	}
	shouldPingDB := os.Getenv("KEEP_ALIVE")
	if shouldPingDB == "1" {
		db.SetMaxIdleConns(1)
	}
	return db
}

func getDatabase() (*sql.DB, error) {
	return sql.Open("postgres", connectionString())
}

func connectionString() string {
	databaseUrl := os.Getenv("DB_URL")
	if databaseUrl != "" {
		if strings.Contains(databaseUrl, "sslmode") {
			return databaseUrl
		}

		return fmt.Sprintf("%s?sslmode=disable", databaseUrl)
	}

	return fmt.Sprintf("host=%s port=%s user=%s password=%s dbname=%s sslmode=disable",
		os.Getenv("DB_HOST"), os.Getenv("DB_PORT"), os.Getenv("DB_USER"), os.Getenv("DB_PASS"), os.Getenv("DB_NAME"))
}

func Poll(DB *sql.DB, stop chan struct{}) chan struct{} {
	stopAck := make(chan struct{})

	go func() {
		defer close(stopAck)
		for {
			select {
			default:
				rows, err := DB.Query(`SELECT 1 FROM users LIMIT 1`)
				if err != nil {
					log.Println("ERROR DB connection failed. Exiting.")
					log.Println(err.Error())
					return
				}
				rows.Close()
				fmt.Println("DB Ping")
				// Every 10 seconds
				time.Sleep(time.Millisecond * 10000)
			case <-stop:
				fmt.Println("Stopping DB Polling")
				return
			}
		}
	}()

	return stopAck
}

//TODO: Take these out of this package and put it somewhere in dbutil

const (
	TABLE_REFUND_REQUESTS          = "refund_requests"
	TABLE_VENDING_MACHINE_REQUESTS = "vending_machine_requests"
	TABLE_BALANCES                 = "balances"
	TABLE_TRANSACTIONS             = "transactions"
)

//Represents a special value provided by the underlying database. Examples are as below
type DatabaseVariable string

const CurrentTimestampUtc DatabaseVariable = `CURRENT_TIMESTAMP AT TIME ZONE 'utc'`
const UuidGenerateV4 DatabaseVariable = "uuid_generate_v4()"

const LastVal DatabaseVariable = "lastval()"
const NULL DatabaseVariable = "NULL"

func NextVal(sequenceName string) DatabaseVariable {
	return DatabaseVariable(fmt.Sprintf(`nextval('%v')`, sequenceName))
}

func (self DatabaseVariable) String() string {
	return string(self)
}

func IsDatabaseVariable(value interface{}) bool {
	switch value.(type) {
	case DatabaseVariable:
		return true
	}
	return false
}

func IsTransaction(ds Quereable) bool {
	switch ds.(type) {
	case TxDataSource:
		return true
	}
	return false
}

type WhereClause struct {
	ColumnName  string
	ColumnValue interface{}
	Condition   Condition
}

func BuildWhereBody(clauses []WhereClause, withParameterStartIndex int) (whereClauseColumnValues []interface{}, whereClause string) {

	var whereClausePlaceholderText []string
	for _, where := range clauses {
		var valueText string
		if IsDatabaseVariable(where.ColumnValue) {
			databaseVariable := where.ColumnValue.(DatabaseVariable)
			valueText = "=" + databaseVariable.String()
		} else {
			valueText = "=$" + strconv.Itoa(withParameterStartIndex)
			whereClauseColumnValues = append(whereClauseColumnValues, where.ColumnValue)
			withParameterStartIndex += 1
		}
		whereClausePlaceholderText = append(whereClausePlaceholderText, where.ColumnName+valueText)
		if len(string(where.Condition)) != 0 {
			whereClausePlaceholderText = append(whereClausePlaceholderText, string(where.Condition))
		}
	}
	return whereClauseColumnValues, strings.Join(whereClausePlaceholderText, " ")
}

type Condition string

const (
	And Condition = "AND"
	Or  Condition = "OR"
)
const (
	AllColumnsAsAsterisk  = "*"
	CountOne              = "COUNT(1)"
	CountOneAsRecordCount = CountOne + " AS record_count"
)

type QueryRowTask interface {
	QueryRow(db *sql.DB) (*sql.Row, string, error)
}

type QueryTask interface {
	Query(db *sql.DB) (*sql.Rows, string, error)
}

type ExecTask interface {
	Exec(db *sql.DB) (int64, error)
}

type RowScanner interface {
	Scan(...interface{}) error
}
type RowsScanner interface {
	io.Closer
	RowScanner
	Next() bool
}

type DataSource interface {
	Quereable
	Begin() (*sql.Tx, error)
}

type Quereable interface {
	Prepare(string) (*sql.Stmt, error)
	QueryRow(string, ...interface{}) *sql.Row
	Query(string, ...interface{}) (*sql.Rows, error)
}

type TxDataSource interface {
	Quereable
	Commit() error
	Rollback() error
}

type Statement interface {
	io.Closer
	Exec(...interface{}) (sql.Result, error)
}
