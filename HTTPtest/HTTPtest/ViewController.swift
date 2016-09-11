//
//  ViewController.swift
//  HTTPtest
//
//  Created by JiangShuli on 9/10/16.
//  Copyright © 2016 SJ. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var Switch: UISwitch!
    @IBOutlet weak var Button: UIButton!
    @IBAction func SendButtonPressed(sender: AnyObject) {

        
        let request = NSMutableURLRequest(URL: NSURL(string: "http://localhost:10002")!)
        request.HTTPMethod = "POST"
        let postString = "_Client_30_0"
        request.HTTPBody = postString.dataUsingEncoding(NSUTF8StringEncoding)
        let task = NSURLSession.sharedSession().dataTaskWithRequest(request) { data, response, error in
            guard error == nil && data != nil else {                                                          // check for fundamental networking error
                print("error=\(error)")
                return
            }
            
            if let httpStatus = response as? NSHTTPURLResponse where httpStatus.statusCode != 200 {           // check for http errors
                print("statusCode should be 200, but is \(httpStatus.statusCode)")
                print("response = \(response)")
            }
            
            let responseString = NSString(data: data!, encoding: NSUTF8StringEncoding)
            print("responseString = \(responseString)")
        }
        task.resume()
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

