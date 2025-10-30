CROW_ROUTE(app, "/update").methods("POST"_method)
([](const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON");
    }

    std::lock_guard<std::mutex> lock(data_mutex);
    latestData.turbidity   = json["turbidity"].d();
    latestData.ph          = json["ph"].d();
    latestData.air_quality = json["air_quality"].d();
    latestData.temperature = json["temperature"].d();
    latestData.water_level = json["water_level"].d();

    return crow::response(200, "Data updated");
});

CROW_ROUTE(app, "/data")
([](){
    std::lock_guard<std::mutex> lock(data_mutex);
    crow::json::wvalue res;
    res["turbidity"]   = latestData.turbidity;
    res["ph"]          = latestData.ph;
    res["air_quality"] = latestData.air_quality;
    res["temperature"] = latestData.temperature;
    res["water_level"] = latestData.water_level;
    return res;
});

app.port(5000).multithreaded().run();