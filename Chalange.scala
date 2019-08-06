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

implicit def converterTransactionDataFromPSApi(entry: Future[Either[ErrorResult, PaginatedSequence[TransactionDataApi]]])(implicit lang: Lang, executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, PaginatedSequence[TransactionData]]] =
    EntityConverter.converterFromApi[PaginatedSequence[TransactionDataApi], PaginatedSequence[TransactionData]](entry)(lang, executionCtx, ctx_m_info, api2transaction_data_ps)


 
 val format = Json.format[Transaction]
 val data_format = Jsonx.formatCaseClass[TransactionData]
}
