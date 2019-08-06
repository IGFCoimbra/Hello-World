package models

import java.time.ZonedDateTime

import com.particeep.api.models.{ ErrorResult, PaginatedSequence, TableSearch }
import com.particeep.api.models.transaction.{ TransactionSearch, Transaction => TransactionApi, TransactionData => TransactionDataApi, TransactionEdition => TransactionEditionApi }
import com.particeep.api.models.enums.Currency.{ Currency, EUR }
import com.particeep.api.models.enums.PaymentMethod.PaymentMethod
import com.particeep.api.models.enums.TransactionStatus.{ PENDING, TransactionStatus }
import converter.EntityConverter
import helpers.sorus.Fail
import models.enums.FundraiseType.FundraiseType
import org.cvogt.play.json.Jsonx
import play.api.i18n.Lang
import play.api.libs.json.{ JsArray, JsObject, Json }
import slick.CopyMacro
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

trait Tests {
  lazy val tests = new tests
}

class tests {

  import TransactionTransformation._
  import EntityConverter.contextCredentialsToApiCredentials

  private[this] val ws = ParticeepApiClient.client

  def byId(id: String)(implicit executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, Transaction]] = {
    ws.transaction(ctx_credentials).byId(id)(executionCtx)
  }

  def byIds(ids: List[String])(implicit executionContext: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, List[Transaction]]] = {
    ws.transaction(ctx_credentials).byIds(ids)(executionContext)
  }

  def search(criteria: TransactionSearch, table_search: TableSearch)(implicit executionContext: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, PaginatedSequence[TransactionData]]] = {
    ws.transaction(ctx_credentials).search(criteria, table_search)(executionContext)
  }

  def cancel(id: String)(implicit executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, Transaction]] = {
    ws.transaction(ctx_credentials).cancel(id)(executionCtx)
  }

  def update(id: String, transaction_edition_api: TransactionEditionApi)(implicit executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, Transaction]] = {
    ws.transaction(ctx_credentials).update(id, transaction_edition_api)
  }
}

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

  private[this] def api2transaction_list(transaction_api_list: List[TransactionApi]): List[Transaction] = transaction_api_list.map(api2transaction)
  private[this] def api2transaction_data_ps(transaction_data_api_ps: PaginatedSequence[TransactionDataApi]): PaginatedSequence[TransactionData] = PaginatedSequence[TransactionData](transaction_data_api_ps.total_size, transaction_data_api_ps.data.map(api2transactionData))
  private[this] def api2transaction_ps(transaction_api_ps: PaginatedSequence[TransactionApi]): PaginatedSequence[Transaction] = PaginatedSequence[Transaction](transaction_api_ps.total_size, transaction_api_ps.data.map(api2transaction))

  implicit def converterTransactionFromApi(entry: Future[Either[ErrorResult, TransactionApi]])(implicit lang: Lang, executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, Transaction]] =
    EntityConverter.converterFromApi[TransactionApi, Transaction](entry)(lang, executionCtx, ctx_m_info, api2transaction)

  implicit def converterTransactionListFromApi(entry: Future[Either[ErrorResult, List[TransactionApi]]])(implicit lang: Lang, executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, List[Transaction]]] =
    EntityConverter.converterFromApi[List[TransactionApi], List[Transaction]](entry)(lang, executionCtx, ctx_m_info, api2transaction_list)

  implicit def converterTransactionFromPSApi(entry: Future[Either[ErrorResult, PaginatedSequence[TransactionApi]]])(implicit lang: Lang, executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, PaginatedSequence[Transaction]]] =
    EntityConverter.converterFromApi[PaginatedSequence[TransactionApi], PaginatedSequence[Transaction]](entry)(lang, executionCtx, ctx_m_info, api2transaction_ps)

  implicit def converterTransactionDataFromPSApi(entry: Future[Either[ErrorResult, PaginatedSequence[TransactionDataApi]]])(implicit lang: Lang, executionCtx: ExecutionContext, ctx_credentials: ContextCredentials, ctx_m_info: ContextMInfo): Future[Either[Fail, PaginatedSequence[TransactionData]]] =
    EntityConverter.converterFromApi[PaginatedSequence[TransactionDataApi], PaginatedSequence[TransactionData]](entry)(lang, executionCtx, ctx_m_info, api2transaction_data_ps)

  lazy val format = Json.format[Transaction]
  lazy val data_format = Jsonx.formatCaseClass[TransactionData]
}
