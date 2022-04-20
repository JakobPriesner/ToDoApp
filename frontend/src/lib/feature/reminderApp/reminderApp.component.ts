import { Component, OnInit } from "@angular/core";
import { App, List } from "src/lib/data-access/models";
import { BackendService } from 'src/lib/data-access/service/backend.service';

@Component({
    selector: 'reminder',
    templateUrl: './reminderApp.component.html',
    styleUrls: ['./reminderApp.component.scss'],
})
export class ReminderAppComponent implements OnInit {
    app?: App;

    constructor( private backendService: BackendService ) { }

    ngOnInit(): void {
        this.backendService.loadReminderApp().subscribe( (newReminderApp: App) => {
            this.app = newReminderApp;
        });
    }

    onListDelete(id: number): void {
        this.app.selectedList = null;
        const index = this.app.lists.findIndex(
            (list) => list.id === id
        );

        // update Counter
        for (let item of this.app.lists[index].items) {
            if (item.flagged) this.app.flaggedCount --;
            if (this.isToday(item.date)) this.app.todayCount --;
        }

        this.app.lists.splice(index, 1);
        this.backendService.deleteList(id).subscribe();
    }

    onListSelect(): List {
        if (this.app.selectedList == null) return null;
        return this.app.selectedList;
    }

    changeFlagCount(event: any): void {
        let value = (event ? 1 : -1);
        this.app.flaggedCount += value;
    }

    setSelectedList(newList: List): void {
        this.app.selectedList.selected = false;
        this.app.selectedList = newList;
    }

    isToday (someDate: string): boolean {
        const today: string = new Date().toISOString().substring(0, 10);
        return today == someDate;
    }
}
