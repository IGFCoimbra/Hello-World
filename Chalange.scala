package models

import java.time.ZonedDateTime
import utils.NumberUtils
import utils.i18n.m.ContextMInfo

import scala.concurrent.{ ExecutionContext, Future }

case class Test(
  id:              String                = "",
  Type:      Option[ZonedDateTime] = None,
  issue_id:       String                = "",
  issue_type:     String                = "",
  recipient_id:    String                = "",
  recipient_type:  String                = "",
  comment:         Option[String]        = None,
  tag:             Option[String]        = None
)

 
object TransactionTransformation extends DateTransformation {

  def api2transaction(transaction_api: TransactionApi): Transaction = {
    val copy = CopyMacro.copyCaseClass[TransactionApi, Transaction]
    val transaction: Transaction = copy(transaction_api, Transaction())

    transaction.copy(
      id = transaction_api.id,
      fundraise_id = transaction_api.fundraise_id.getOrElse(""),
      fundraise_type = transaction_api.fundraise_type.headOption.flatMap(FundraiseType.get(_)),
      amount = NumberUtils.fromCents(transaction_api.amount),
      fees = NumberUtils.fromCents(transaction_api.fees),
      custom = transaction_api.custom
    )
  }

  def api2transactionData(transaction_data_api: TransactionDataApi): TransactionData = {
    val copy = CopyMacro.copyCaseClass[TransactionDataApi, TransactionData]
    val transaction_data: TransactionData = copy(transaction_data_api, TransactionData())

    transaction_data.copy(
      id = transaction_data_api.id,
      amount = transaction_data_api.amount.map(NumberUtils.fromCents),
      fees = transaction_data_api.fees.map(NumberUtils.fromCents),
      custom = transaction_data_api.custom
    )
  }

  
  lazy val format = Json.format[Transaction]
  lazy val data_format = Jsonx.formatCaseClass[TransactionData]
}
 val data_format = Jsonx.formatCaseClass[TransactionData]
