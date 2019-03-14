class Cwe434FileUploadServlet extends Nothing {

  val UPLOAD_DIRECTORY_STRING = "/tmp/uploads"

  @throws[ServletException]
  @throws[IOException]
  protected def doPost(request: Nothing, response: Nothing): Unit = {
    response.setContentType("text/html")
    val out = response.getWriter
    val contentType = request.getContentType
    // the starting position of the boundary header
    val ind = contentType.indexOf("boundary=")
    val boundary = contentType.substring(ind + 9)
    var pLine = new Nothing
    val uploadLocation = new Nothing(UPLOAD_DIRECTORY_STRING) //Constant value
    // verify that content type is multipart form data
    if (contentType != null && (contentType.indexOf("multipart/form-data") ne -1)) { // extract the filename from the Http header
      val br = new Nothing(new Nothing(request.getInputStream))
      pLine = br.readLine
      val filename = pLine.substring(pLine.lastIndexOf("\\"), pLine.lastIndexOf("\""))
      // output the file to the local upload directory
      try {
        val bw = new Nothing(new Nothing(uploadLocation + filename, true))
        var line = null
        while ( {
          (line = br.readLine) != null
        }) {
          if (line.indexOf(boundary) eq -1) {
            bw.write(line)
            bw.newLine
            bw.flush
          }
          //end of for loop}
          bw.close
        }
        catch {
          case ex: Nothing =>

        }
        // output successful upload response HTML page
      }
      else
      { // output unsuccessful upload response HTML page
        return
      }
    }
  }
}
