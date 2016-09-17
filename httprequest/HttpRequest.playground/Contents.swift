//all request data is in list ["string"]
//if you need to add user
let testparam = String("fun=adduser&andrewid=haohans&username=Haohan-Shi")
//if you want to add course
//role can be either "Student" or "Instructor"
let testparam = String("fun=addcourse&andrewid=haohans&courseid=21259&role=student")
//if you want to get course list
//return a string of courseid eg "21259,76101,"
let testparam = String("fun=getcourselist&andrewid=haohans")
//if you want to add due
//return a list containing string ["Due added on 21259 at 09-17-17-00"]
//if you are a student, return "Permission Denied"
let testparam = String("fun=adddue&andrewid=haohans&duedate=09-17&duetime=17-00&coursecode=21259&hwname=written1&handintype=recitation")
//if you want to get all the due of one course
//return a list which the first element is course name, then due content, all elements are split by ,
//eg: ["Calculus in Three Dimensions", "09-17-17-00-written1-recitation", "09-17-17-00-written1-recitation"]
let testparam = String("fun=getonedue&andrewid=haohans&coursecode=21259")



//This is the template
let testurl = String("http://128.237.183.206:8000?")
let testparam = xxxxxxxxx from above
let finalurl = testurl+testparam
let url = NSURL(string: finalurl)
let request = NSURLRequest(URL: url!)
let config = NSURLSessionConfiguration.defaultSessionConfiguration()
let session = NSURLSession(configuration: config)

let task = session.dataTaskWithRequest(request, completionHandler: {(data, response, error) in
    if let error = error {
        print(error)
        return }
    // Parse JSON data
    if let data = data {
        
        let dataString = NSString(data: data, encoding: NSUTF8StringEncoding)
        let convertdata = dataString as! String
        let stringArr = convertdata.characters.split{$0 == ","}.map(String.init)
        print(stringArr)
    }
});
// do whatever you need with the task e.g. run
task.resume()
