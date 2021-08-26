package main

import (
	"bytes"
	"log"
	"net/http"
	"time"
	"fmt"
    "encoding/json"
	"math/rand"
)

type Request struct {
	PrimeNum int
	Priority int
	DeadlineTime int
}


func main() {
	for i := 0; i < 50; i++ {
		err := call("http://localhost:8080/prime", "POST", i)
		if err != nil {
			panic(err)
		}
	}
	call("http://localhost:8080/prime", "GET", 0)
}

func call(urlPath, method string, requestNum int) error {
	client := &http.Client{
		Timeout: time.Second * 10,
	}

	var task Request
	task.PrimeNum = rand.Intn(9900) + 100
	task.Priority = rand.Intn(12)
	task.DeadlineTime = rand.Intn(10)

	taskJson, err := json.Marshal(task)

	req, err := http.NewRequest(method, urlPath, bytes.NewReader(taskJson))
	if err != nil {
		return err
	}

	rsp, _ := client.Do(req)
	if rsp.StatusCode != http.StatusOK {
		log.Printf("Request failed with response code: %d", rsp.StatusCode)
	}
	fmt.Println(rsp.StatusCode)
	return nil
}
