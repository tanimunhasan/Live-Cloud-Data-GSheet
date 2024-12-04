function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  Logger.log(e.postData.contents);  // Log incoming data to check if it's reaching

  var data = JSON.parse(e.postData.contents);  // Parse JSON data
  
  if (data.value1 !== undefined && data.value2 !== undefined) {
    sheet.appendRow([new Date(), data.value1, data.value2]);
  }
  
  return ContentService.createTextOutput("Data received successfully");
}
