use web_view::*;

pub fn init(){
    println!("oi, eu sou a ui");

    //let html_content = "<html><body><h1>Hello, World!</h1></body></html>";
    let html_content = include_str!("../static/index.min.html");
	
    web_view::builder()
        .title("My Project")
        .content(Content::Html(html_content))
        .size(320, 480)
        .resizable(true)
        .debug(true)
        .user_data(())
        .invoke_handler(|_webview, _arg| Ok(()))
        .run()
        .unwrap();
}