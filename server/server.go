package main

import (
	"net/http"
	"encoding/json"
	"fmt"
)

type Request struct {
	PrimeNum int
	Priority int
	DeadlineTime int
}

type Response struct {
	Num int
}

var tasksScheduler = make(map[int][]Request)

var numRequests = 0

func main() {
	createImageHandler := http.HandlerFunc(createPrime)
	http.Handle("/prime", createImageHandler)
	http.ListenAndServe(":8080", nil)
}

func schedule() {
	fmt.Println(tasksScheduler)
	for _, tasks := range tasksScheduler {
		for _, task := range tasks {
			fmt.Println(generatePrime(task.PrimeNum))
		}
	}
}

func createPrime(w http.ResponseWriter, request *http.Request) {

	if request.Method  == http.MethodGet {
		schedule()
		return
	} else {
		var task Request
		err := json.NewDecoder(request.Body).Decode(&task)
		if err != nil {
			http.Error(w, err.Error(), http.StatusBadRequest)
		}
		tasks := tasksScheduler[task.Priority]
		tasks = append(tasks, task)
		tasksScheduler[task.Priority] = tasks
		// fmt.Println(task.PrimeNum, "\t", generatePrime(task.PrimeNum))
		w.WriteHeader(200)
		return
	}
}

func generatePrime(num int) (out int) {
	out = 2
	for i := 0; i < num; {
		if isPrime(out) {
			i++
		}
		out++
	}
	return out - 1
}

func isPrime(num int) bool {
	for i := 2; i < num; i++ {
		if num % i == 0 {
			return false
		}
	}
	return true
}
