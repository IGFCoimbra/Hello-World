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
